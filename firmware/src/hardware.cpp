#include "hardware.hpp"
#include "stm32_lib/hl_rcc.hpp"
#include "stm32_lib/hl_flash.hpp"
#include "stm32_lib/hl_dwt.hpp"

#include "gui.hpp"
#include "sound.hpp"


using namespace hl;

void hardware_init_clock()
{
	rcc_enable_hsi();
	rcc_set_sys_clock(SystemClock::HSI);

	rcc_enable_hse(false);

	flash_enable_prefetch();

	rcc_set_apb2_prescaler(APB2Prescaler::_1);
	rcc_set_apb1_prescaler(APB1Prescaler::_1);
	rcc_set_ahb_prescaler(AHBPrescaler::_1);

	rcc_disable_ppl();
	rcc_set_ppl_source(PPLSource::HSE);
	rcc_disable_hse_div2_for_pll();
	rcc_set_ppl_mult_factor(PPLMult::_6);
	rcc_enable_ppl();

	rcc_set_sys_clock(SystemClock::PLL);

	rcc_disable_hsi();
}

void hardware_base_init()
{
	PA::clock_on();
	PA::reset();

	PB::clock_on();
	PB::reset();

	PC::clock_on();
	PC::reset();

	DWTCounter::enable();

	WhiteLed::conf_out_push_pull();
	WhiteLed::on();
}


void hardware_init_ticks_timer()
{
	// Инициализируем таймер для запуска замера
	TicksTimer::clock_on();
	TicksTimer::reset();
	TicksTimer::disable_auto_reload_preload();
	TicksTimer::set_prescaler(SysFreq/1000-1); // 1 мс
	TicksTimer::set_auto_reload_value(10-1); // 10 мс
	TicksTimer::generate_update();

	TicksTimer::enable_update_interrupt();
	TicksTimer::clear_all_flags();
	TicksTimer::start();

	NVIC_EnableIRQ(TicksTimer::IRQn);
}

extern "C" void TIM2_IRQHandler()
{
	if (TicksTimer::get_update_interrupt_flag())
	{
		gui_tick_handler();
		sound_tick_handler();

		TicksTimer::clear_update_interrupt_flag();
	}
}
