#include <algorithm>
#include <iterator>
#include "gui.hpp"
#include "hardware.hpp"
#include "co2unit.hpp"
#include "config.hpp"

#include "stm32_lib/hl_dwt.hpp"
#include "stm32_lib/hl_tim.hpp"
#include "stm32_lib/hl_dma.hpp"

#include "muil_basic_gui.hpp"
#include "muil_font_firasans_8_ru.hpp"
#include "muil_font_firasans_20_digits.hpp"
#include "muil_utils.hpp"

#include "ledmatrix_lib/hub75_rgb_controller.hpp"
#include "ledmatrix_lib/hub75_rgb_screen.hpp"

using namespace muil;
using namespace hl;

//============================ LED-screen hardware =============================

namespace lm = ledmatrix;

using Screen = lm::Hub75RGBScreen555<1, 1, 64, 64>;

constexpr unsigned TimersClock = SysClockFreq;
constexpr unsigned RowScanPeriod = 400;

typedef hl::PB Hub75Port;

typedef hl::PB0 R1Pin;
typedef hl::PB1 R2Pin;
typedef hl::PB3 G1Pin;
typedef hl::PB4 G2Pin;
typedef hl::PB5 B1Pin;
typedef hl::PB6 B2Pin;
typedef hl::PB7 APin;
typedef hl::PB8 BPin;
typedef hl::PB9 CPin;
typedef hl::PB10 DPin;
typedef hl::PB11 EPin;
typedef hl::PB12 OEPin;
typedef hl::PB13 LatPin;
typedef hl::PB14 ClkPin;

typedef hl::Timer2 DmaStransTimer;
typedef hl::Dma1_Controller TransDma;
typedef hl::Dma1_Chan2 TransDmaChan;

typedef hl::Timer1 RowScanTimer;
#define RowScanTimer_IRQHandler TIM1_UP_IRQHandler

typedef hl::Timer3 BamTimer;
#define BamTimer_IRQHandler TIM3_IRQHandler

using Pins = lm::Scan32Pins<
	R1Pin, R2Pin, G1Pin, G2Pin, B1Pin, B2Pin,
	APin, BPin, CPin, DPin, EPin,
	OEPin, LatPin, ClkPin
>;

using Line = lm::Hub75LineHardware<
	Hub75Port,
	Pins,
	TransDmaChan, 
	-1, // DMA channel index. Used in STM32F4 DMA controllers
	-1  // Timer PWM channel for Memory -> DMA -> GPIO transfer. -1 is for using update event
>;

using LineHardwareItems = lm::Hub75LineHardwareItems<Line>;

using Controller = lm::Hub75Controller<
	TimersClock,
	RowScanPeriod,
	Screen,
	LineHardwareItems, 
	TransDma,
	DmaStransTimer,
	RowScanTimer,
	BamTimer,
	15
>;


static Screen screen;
static Controller controller { screen };

extern "C" void RowScanTimer_IRQHandler()
{
	DebugPin::on();
	controller.process_row_scan_timer_interrupt();	
	DebugPin::off();
}

extern "C" void BamTimer_IRQHandler()
{
	controller.process_bam_timer_interrupt();
}


//========================= LED-screen implementation ==========================

unsigned muil::display_get_width()
{
	return Screen::width;
}

unsigned muil::display_get_height()
{
	return Screen::height;
}

void muil::display_set_point(
	int         x, 
	int         y, 
	const Color &color)
{
	if (x < 0) return;
	if (x >= (int)Screen::width) return;
	if (y < 0) return;
	if (y >= (int)Screen::height) return;
	screen.set_pixel(x, y, color.r, color.g, color.b);
}


void muil::display_paint_character(
	int           x0, 
	int           y0, 
	const uint8_t *data, 
	uint8_t       width, 
	uint8_t       height, 
	const Color   &color, 
	const Color   *bg_color)
{
	default_display_paint_character(x0, y0, data, width, height, color, bg_color);
}

void muil::display_fill_rect(
	const Rect  &rect, 
	const Color &color)
{
	default_display_fill_rect(rect, color);
}

//==============================================================================


void muil::delay_ms(uint16_t milliseconds)
{
	Delay<SysClockFreq>::exec_ms(milliseconds);
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
constexpr unsigned DayValuesCnt = Screen::width;
static uint16_t day_values[DayValuesCnt];
static uint16_t day_values_ptr = 0;

Color get_color_for_value(
	uint16_t    value,
	uint16_t    ok_value,
	uint16_t    warn_value,
	uint16_t    alarm_value,
	const Color &good_color,
	const Color &warn_color,
	const Color &alarm_color)
{
	if (value <= ok_value) return good_color;
	if (value >= alarm_value) return alarm_color;

	uint16_t v1 = 0;
	uint16_t v2 = 0;
	const Color *color1 = nullptr;
	const Color *color2 = nullptr;
	
	if (value < warn_value)
	{
		v1 = ok_value;
		v2 = warn_value;
		color1 = &good_color;
		color2 = &warn_color;
	}
	else
	{
		v1 = warn_value;
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
	Controller::init();
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
	
	Controller::start();
}


void gui_show_welcome_screen()
{
	display_fill_rect(0, 0, display_get_width()-1, display_get_height()-1, Color(0, 0, 16));

	auto &text_font = firaSans8ptFontInfo;
	auto font_height = display_get_font_height(text_font);

	const Color text_color {255, 255, 255};

	int y = 1;
	int x = 2;

	display_print_string(x, y, u8"Индикатор", text_font, text_color, nullptr);
	y += font_height;
	
	display_print_string(x, y, u8"CO2", text_font, text_color, nullptr);
	y += 4*font_height/3;

	display_print_string(x, y, u8"Диапазон:", text_font, text_color, nullptr);
	y += font_height;
	
	display_print_string(x, y, u8"0..5000 ppm", text_font, text_color, nullptr);
	y += font_height;
}

static void show_graphics(const Rect &rect, unsigned min_max)
{
	const Color bk_color {0, 0, 0};
	const Color good_color { 0, 64, 0 };
	const Color warn_color { 64, 64, 0 };
	const Color bad_color { 64, 0, 0 };
	const Color text_color {32, 32, 32};
	const Color yellow(32, 32, 0);
	const Color red(32, 0, 0);
	const Color green(0, 32, 0);

	unsigned max = *std::max_element(std::begin(day_values), std::end(day_values));

	if (max < min_max) max = min_max;

	if (max < 1000) max = 1000;
	else if (max < 2000) max = 2000;
	else if (max < 4000) max = 4000;
	else if (max < 8000) max = 8000;

	auto y_fun = [&] (int value) { return rect.y2 - value * (rect.y2 - rect.y1) / max; };

	int ok_y = y_fun(config.ok_value);
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
			config.ok_value,
			config.warn_value,
			config.alarm_value,
			good_color,
			warn_color,
			bad_color
		);

		display_fill_rect(x, y, x, rect.y2, value_color);

		if ((x % 2) == 0)
		{
			if (ok_y > rect.y1)
				display_set_point(x, ok_y, green);
			if (warn_y > rect.y1)
				display_set_point(x, warn_y, yellow);
			if (alarm_y > rect.y1)
				display_set_point(x, alarm_y, red);
				
			display_set_point(x, rect.y1, text_color);
		}
	}

	auto &text_font = firaSans8ptFontInfo;
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
	auto &value_font = firaSans20ptFontInfo;
	auto &text_font = firaSans8ptFontInfo;

	const Color good_color { 0, 255, 0};
	const Color warn_color { 255, 255, 0 };
	const Color bad_color { 255, 0, 0 };

	const Color bk_color = {0, 0, 0};
	Color value_color;

	const uint16_t value_font_height = display_get_font_height(value_font);
	const uint16_t text_font_height = display_get_font_height(text_font);

	if (after_start_counter == 0)
		value_color = get_color_for_value(
			value,
			config.ok_value,
			config.warn_value,
			config.alarm_value,
			good_color,
			warn_color,
			bad_color
		);
	else
		value_color = Color(0, 100, 100);

	int y = 0;

	// Текущее значение крупным шрифтом
	Integer10CharactersProvider value_provider(value, -1);
	Rect value_rect(0, y, display_get_width()-1, y+value_font_height+2);
	y += value_rect.height();

	display_print_any_in_rect(
		value_rect,
		HorizAlign::Center,
		value_provider,
		value_font,
		value_color,
		&bk_color
	);

	// График за сутки
	Rect gr_rect(0, y, display_get_width()-1, display_get_height()-1);
	show_graphics(gr_rect, std::max(config.warn_value, config.warn_value));
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
				set_lcd_led_flashing(co2_value);
			}
			show_measure(co2_value);
			last_measures_count = measures_count;
		}

		Delay<SysClockFreq>::exec_ms(100);
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
}