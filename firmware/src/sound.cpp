#include "sound.hpp"

#include "hardware.hpp"
#include "stm32_lib/hl_tim.hpp"

using namespace hl;

enum class Type
{
	None,
	Warn,
	Alarm
};

static volatile Type type;
static volatile Type cur_type;
static volatile unsigned ordered_beep_count;
static volatile unsigned ticks_counter = 0;
static volatile unsigned beep_counter = 0;

constexpr unsigned reload_value = 10;

void sound_init()
{
	SoundTimer::clock_on();
	SoundTimer::reset();

	SoundPin::conf_alt_push_pull();

	SoundTimer::disable_auto_reload_preload();
	SoundTimer::set_prescaler(SysFreq/10000-1); // 100 мкс
	SoundTimer::set_auto_reload_value(reload_value-1); // 1 мс
	SoundTimer::conf_pwm<SoundTimerChan>(PWMMode::_1, PWMPolarity::High, PWMPreload::Disable);
	SoundTimer::set_pwm_value<SoundTimerChan>(0);

	type = Type::None;
	cur_type = Type::None;
	ticks_counter = 0;
	ordered_beep_count = 0;
	beep_counter = 0;
}

void sound_do_not_play()
{
	type = Type::None;
}

void sound_play_warn(unsigned beep_count)
{
	ordered_beep_count = beep_count;
	if (type == Type::Warn) return;
	if (type == Type::None) beep_counter = 0;
	type = Type::Warn;
}

void sound_play_alarm()
{
	if (type == Type::Alarm) return;
	type = Type::Alarm;
	if (type == Type::None) beep_counter = 0;
	ordered_beep_count = 1000000;
}

void sound_tick_handler()
{
	if (cur_type != type)
	{
		cur_type = type;
		if (type == Type::None)
		{
			SoundTimer::stop();
			SoundTimer::set_pwm_value<SoundTimerChan>(0);
			return;
		}
	}

	if (cur_type == Type::None) return;

	ticks_counter++;
	if ((ticks_counter == 1) && (beep_counter < ordered_beep_count))
	{
		SoundTimer::set_pwm_value<SoundTimerChan>(reload_value/4);
		SoundTimer::start();
	}

	if ((ticks_counter == 20) && (beep_counter < ordered_beep_count))
	{
		SoundTimer::stop();
		SoundTimer::set_pwm_value<SoundTimerChan>(0);
	}

	if (ticks_counter == 50)
	{
		ticks_counter = 0;
		beep_counter++;
		if (beep_counter == 10) beep_counter = 0;
	}
}
