#include "stm32_lib/hl_rcc.hpp"
#include "stm32_lib/hl_flash.hpp"
#include "stm32_lib/hl_dwt.hpp"
#include "stm32_lib/hl_misc.hpp"
#include "hardware.hpp"

#include "gui.hpp"


using namespace hl;

void hardware_init_clock()
{
	misc_f1_conf_sysclock_hse_high(
		PLLMult::_12,
		APB2Prescaler::_1,
		APB1Prescaler::_1,
		AHBPrescaler::_1,
		FlashLatency::_2
	);
}

void hardware_base_init()
{
	PA::clock_on();
	PA::reset();

	PC::clock_on();
	PC::reset();

	dwt_enable();

	WhiteLed::conf_out_push_pull();
	WhiteLed::on();
	
	DebugPin::conf_out_push_pull();
	
	rcc_enable_afio();
	gpio_disable_jtag(); // чтобы PB4 не ремапился на JTAG
}


void hardware_init_ticks_timer()
{
	// Инициализируем таймер для запуска замера и GUI
	TicksTimer::clock_on();
	TicksTimer::reset();
	TicksTimer::disable_auto_reload_preload();
	TicksTimer::set_prescaler(SysClockFreq/1000000u-1u); // 10 мс
	TicksTimer::set_auto_reload_value(10000-1); // 100 мс
	TicksTimer::generate_update();
	
	NVIC_SetPriority(TicksTimer::IRQn, 4);

	TicksTimer::enable_update_interrupt();
	TicksTimer::clear_all_flags();
	TicksTimer::start();

	NVIC_EnableIRQ(TicksTimer::IRQn);
}

extern "C" void TicksTimer_IRQHandler()
{
	if (TicksTimer::get_update_interrupt_flag())
	{
		gui_tick_handler();

		TicksTimer::clear_update_interrupt_flag();
	}
}
