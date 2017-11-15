#include <algorithm>
#include <iterator>
#include "gui.hpp"
#include "hardware.hpp"
#include "co2unit.hpp"
#include "sound.hpp"
#include "config.hpp"

#include "stm32_lib/hl_dwt.hpp"
#include "stm32_lib/hl_tim.hpp"

#include "muil_font_firasans_16_ru.hpp"
#include "muil_font_firamono_72_digits.hpp"
#include "muil_utils.hpp"

using namespace muil;
using namespace hl;

static DisplayDriver display;

IMPLEMENT_ILI9341_DISPLAY(display)

void muil::delay_ms(uint16_t milliseconds)
{
	Delay<SysFreq>::exec_ms(milliseconds);
}

constexpr unsigned HistoryHours = 12;
constexpr unsigned AfterStartIgnorePeriod = 4*60; // секунд


static volatile unsigned start_measure_cnt = 0;
static volatile unsigned after_start_counter = 0;
static volatile unsigned aver_cnt = 0;
static volatile unsigned aver_acc = 0;
static volatile unsigned aver_summ = 0;

enum class LedMode
{
	Undef,
	NoFlashing,
	Flashing
};

static volatile LedMode led_mode;

// История замеров
constexpr unsigned DayValuesCnt = 240;
static uint16_t day_values[DayValuesCnt];
static uint16_t day_values_ptr = 0;

Color get_color_for_value(
	uint16_t    value,
	uint16_t    warn_value,
	uint16_t    alarm_value,
	const Color &good_color,
	const Color &warn_color,
	const Color &alarm_color)
{
	if (value <= warn_value) return good_color;
	if (value >= alarm_value) return alarm_color;

	uint16_t v1 = 0;
	uint16_t v2 = 0;
	const Color *color1 = nullptr;
	const Color *color2 = nullptr;
	uint16_t middle_value = (warn_value + alarm_value) / 2;
	if (value < middle_value)
	{
		v1 = warn_value;
		v2 = middle_value;
		color1 = &good_color;
		color2 = &warn_color;
	}
	else
	{
		v1 = middle_value;
		v2 = alarm_value;
		color1 = &warn_color;
		color2 = &alarm_color;
	}

	int r = muil::linear_interpol(value, v1, v2, color1->r, color2->r);
	int g = muil::linear_interpol(value, v1, v2, color1->g, color2->g);
	int b = muil::linear_interpol(value, v1, v2, color1->b, color2->b);
	if (r < 0) r = 0;
	if (r > 255) r = 0;
	if (g < 0) g = 0;
	if (g > 255) g = 0;
	if (b < 0) b = 0;
	if (b > 255) b = 0;
	return Color(r, g, b);
}

void gui_init_hardware()
{
	// Инициализируем пины SPI дисплея
	DisplayCSPin::conf_out_push_pull();
	DisplayDCPin::conf_out_push_pull();
	DisplayResetPin::conf_out_push_pull();

	DisplaySCKPin::conf_alt_push_pull();
	DisplayMISOPin::conf_in();
	DisplayMOSIPin::conf_alt_push_pull();

	// Инициализируем SPI для дисплея
	LCD_SPI::clock_on();
	LCD_SPI::reset();

	LCD_SPI::init(
		SPI_BitsCount::_8,
		SPI_CSMode::Soft,
		SPI_CPHA::_2_Edge,
		SPI_CPOL::High,
		SPI_BRPrescaler::_2
	);

	display.init();

	// Таймер и пин для регулировки якрости дисплея через ШИМ
	DisplLedTimer::clock_on();
	DisplLedTimer::reset();
	DisplLedPin::conf_alt_push_pull();
	DisplLedTimer::disable_auto_reload_preload();
	DisplLedTimer::set_prescaler(SysFreq/10000-1); // 100 мкс
	DisplLedTimer::set_auto_reload_value(20-1); // 2 мс
	DisplLedTimer::conf_pwm<DisplLedTimerChan>(PWMMode::_2, PWMPolarity::High, PWMPreload::Disable);
	DisplLedTimer::set_pwm_value<DisplLedTimerChan>(0);
	DisplLedTimer::start();
}

void gui_init()
{
	// Массив для хранения истории
	day_values_ptr = 0;
	std::fill(std::begin(day_values), std::end(day_values), 0);

	start_measure_cnt = 0;
	after_start_counter = AfterStartIgnorePeriod * 100;
	aver_cnt = 0;
	aver_acc = 0;
	aver_summ = 0;
	led_mode = LedMode::Undef;
}

enum class LedIntensity
{
	Low,
	Middle,
	Full
};

static void set_display_led_intensity(LedIntensity intensity)
{
	switch (intensity)
	{
	case LedIntensity::Low:
		DisplLedTimer::set_pwm_value<DisplLedTimerChan>(2);
		break;

	case LedIntensity::Middle:
		DisplLedTimer::set_pwm_value<DisplLedTimerChan>(5);
		break;

	case LedIntensity::Full:
		DisplLedTimer::set_pwm_value<DisplLedTimerChan>(19);
		break;
	}
}


void gui_show_welcome_screen()
{
	display_fill_rect(0, 0, display_get_width()-1, display_get_height()-1, Color(0, 0, 128));

	auto &text_font = firaSans16ptFontInfo;
	auto font_height = display_get_font_height(text_font);

	const Color text_color {255, 255, 255};

	int y = 10;
	int x = 10;

	display_print_string(x, y, u8"Индикатор CO2", text_font, text_color, nullptr);
	y += 3 * font_height / 2;

	display_print_string(x, y, u8"Диапазон: 0 ... 5000 ppm", text_font, text_color, nullptr);
	y += 2 * font_height;

	display_print_string(x, y, u8"Не дышите на датчик :-)", text_font, text_color, nullptr);
	y += 3 * font_height / 2;

	set_display_led_intensity(LedIntensity::Low);
}

static void show_graphics(const Rect &rect, unsigned min_max)
{
	const Color bk_color {255, 255, 255};
	const Color good_color { 32, 192, 32 };
	const Color warn_color { 192, 192, 32 };
	const Color bad_color { 192, 32, 32 };
	const Color grid_color {0, 0, 0};
	const Color text_color {100, 100, 100};

	unsigned max = *std::max_element(std::begin(day_values), std::end(day_values));

	if (max < min_max) max = min_max;

	if (max < 1000) max = 1000;
	else if (max < 2000) max = 2000;
	else if (max < 4000) max = 4000;
	else if (max < 8000) max = 8000;

	auto y_fun = [&] (int value) { return rect.y2 - value * (rect.y2 - rect.y1) / max; };

	int warn_y = y_fun(config.warn_value);
	int alarm_y = y_fun(config.alarm_value);

	int ptr = day_values_ptr;
	for (int x = std::size(day_values)-1; x >= 0 ; x--)
	{
		ptr--;
		if (ptr == -1) ptr = std::size(day_values)-1;
		int value = day_values[ptr];
		int y = y_fun(value);

		display_fill_rect(x, rect.y1, x, y-1, bk_color);

		Color value_color = get_color_for_value(
			value,
			config.warn_value,
			config.alarm_value,
			good_color,
			warn_color,
			bad_color
		);

		display_fill_rect(x, y, x, rect.y2, value_color);

		if ((x % 4) == 0)
		{
			for (int i = 0; i <= 3; i++)
			{
				int py = i * (rect.y2 - rect.y1) / 4 + rect.y1;
				display_set_point(x, py, grid_color);
			}
		}

		if (((x % 20) == 0) && (x != 0))
		{
			for (int y = rect.y1; y < rect.y2; y += 4)
				display_set_point(x, y, grid_color);
		}

		if ((x % 2) == 0)
		{
			if (warn_y > rect.y1)
			{
				const Color yellow(255, 255, 0);
				display_set_point(x, warn_y-1, yellow);
				display_set_point(x, warn_y, yellow);
				display_set_point(x, warn_y+1, yellow);
			}
			if (alarm_y > rect.y1)
			{
				const Color red(255, 0, 0);
				display_set_point(x, alarm_y-1, red);
				display_set_point(x, alarm_y, red);
				display_set_point(x, alarm_y+1, red);
			}
		}
	}

	auto &text_font = firaSans16ptFontInfo;
	auto text_font_height = display_get_font_height(text_font);

	display_print_integer(rect.x1+3, rect.y1, max, -1, text_font, text_color, nullptr);
	display_print_integer(rect.x1+3, rect.y2-text_font_height-1, 0, -1, text_font, text_color, nullptr);
}

static void add_value_to_graphics(unsigned value)
{
	aver_acc += value;
	aver_cnt++;

	unsigned per_hour_gr_count = DayValuesCnt / HistoryHours;
	unsigned per_hour_measures_count = 3600 / config.request_period;

	aver_summ += per_hour_gr_count;

	if (aver_summ >= per_hour_measures_count)
	{
		aver_summ -= per_hour_measures_count;

		day_values[day_values_ptr] = aver_cnt ? (aver_acc + aver_cnt/2) / aver_cnt : 0;
		day_values_ptr++;
		if (day_values_ptr >= std::size(day_values)) day_values_ptr = 0;

		aver_acc = 0;
		aver_cnt = 0;
	}
}

static void show_measure(unsigned value)
{
	auto &value_font = firaMono72ptFontInfo;
	auto &text_font = firaSans16ptFontInfo;

	const Color good_color { 96, 255, 96 };
	const Color warn_color { 192, 192, 32 };
	const Color bad_color { 255, 96, 96 };

	Color value_color {0, 0, 0};

	const uint16_t value_font_height = display_get_font_height(value_font);
	const uint16_t text_font_height = display_get_font_height(text_font);

	Color bk_color;

	if (after_start_counter == 0)
		bk_color = get_color_for_value(
			value,
			config.warn_value,
			config.alarm_value,
			good_color,
			warn_color,
			bad_color
		);
	else
		bk_color = Color(128, 190, 255);

	int y = 0;

	// Надпись "Текущие значения CO2"
	const char *measure_caption = (after_start_counter != 0) ? u8"Значение с датчика" : u8"Замер CO2 (ppm)";
	Utf8CharactersProvider cur_co2_text_provider(measure_caption);
	Rect cur_co2_text_rect(0, y, display_get_width()-1, y + 3*text_font_height/2);
	y += cur_co2_text_rect.height();
	display_print_any_in_rect(
		cur_co2_text_rect,
		HorizAlign::Center,
		cur_co2_text_provider,
		text_font,
		value_color,
		&bk_color
	);

	// Текущее значение крупным шрифтом
	Integer10CharactersProvider value_provider(value, -1);
	Rect value_rect(0, y, display_get_width()-1, y+35*value_font_height/20);
	y += value_rect.height();

	display_print_any_in_rect(
		value_rect,
		HorizAlign::Center,
		value_provider,
		value_font,
		value_color,
		&bk_color
	);

	// Надпись "График за сутки"
	Color gr_cap_bk_color {255, 255, 255};
	Utf8CharactersProvider gr_cap_text_provider(u8"График за 12 часов");
	Rect gr_cap_text_rect (0, y, display_get_width()-1, y + 3*text_font_height/2);
	y += gr_cap_text_rect.height();

	display_print_any_in_rect(
		gr_cap_text_rect,
		HorizAlign::Center,
		gr_cap_text_provider,
		text_font,
		value_color,
		&gr_cap_bk_color
	);

	// График за сутки
	Rect gr_rect(0, y, display_get_width()-1, display_get_height()-1);
	show_graphics(gr_rect, std::max(config.warn_value, config.warn_value));
}

static void play_sound(unsigned value)
{
	if (value < config.warn_value)
		sound_do_not_play();
	else if ((value >= config.warn_value) && (value < config.alarm_value))
	{
		unsigned beeps_counter = muil::linear_interpol<unsigned, unsigned>(
			value,
			config.warn_value,
			config.alarm_value,
			1,
			4
		);
		sound_play_warn(beeps_counter);
	}
	else if (value >= config.alarm_value)
		sound_play_alarm();
}

static void set_lcd_led_flashing(unsigned value)
{
	if (value > config.alarm_value)
		led_mode = LedMode::Flashing;
	else
		led_mode = LedMode::NoFlashing;
}

void gui_run()
{
	unsigned last_measures_count = 0;

	for (;;)
	{
		unsigned measures_count = co2unit_get_measures_count();
		if (measures_count != last_measures_count)
		{
			WhiteLed::on();
			auto co2_value = co2unit_get_result();
			if (after_start_counter == 0)
			{
				add_value_to_graphics(co2_value);
				play_sound(co2_value);
				set_lcd_led_flashing(co2_value);
			}
			show_measure(co2_value);
			last_measures_count = measures_count;
		}

		Delay<SysFreq>::exec_ms(100);
	}
}

static volatile unsigned ticks_count = 0;

void gui_tick_handler()
{
	ticks_count++;

	if (after_start_counter != 0) after_start_counter--;

	start_measure_cnt++;
	if (start_measure_cnt >= config.request_period * 100)
	{
		start_measure_cnt = 0;
		co2unit_start_measure();
		WhiteLed::off();
	}

	if ((ticks_count & 31) == 0)
	{
		switch (led_mode)
		{
		case LedMode::NoFlashing:
			set_display_led_intensity(LedIntensity::Middle);
			break;

		case LedMode::Flashing:
			if ((ticks_count & 32) == 0)
				set_display_led_intensity(LedIntensity::Full);
			else
				set_display_led_intensity(LedIntensity::Middle);
			break;
		}
	}
}