#pragma once

#include <stm32l1xx.h>

namespace hl {

inline void pwr_enable_interface()
{
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	__DSB();
}


// low power mode

inline void pwr_enable_low_power_in_run_mode()
{
	PWR->CR |= PWR_CR_LPSDSR;
	PWR->CR |= PWR_CR_LPRUN;

}

inline void pwr_enable_low_power_in_sleep_mode()
{
	PWR->CR &= PWR_CR_LPSDSR;
	PWR->CR |= PWR_CR_LPRUN;
}

inline void pwr_disable_low_power_mode()
{
	PWR->CR &= ~PWR_CR_LPRUN;
	PWR->CR &= ~PWR_CR_LPSDSR;
}


// V core

enum class VCore
{
	_1_8 = 0b01 << 11, // 1.8 V (range 1)
	_1_5 = 0b10 << 11, // 1.5 V (range 2)
	_1_2 = 0b11 << 11, // 1.2 V (range 3)
};

inline void pwr_set_v_core(VCore value)
{
	uint32_t cr = PWR->CR;
	cr &= ~PWR_CR_VOS;
	cr |= (uint32_t)value;
	PWR->CR = cr;
	while (PWR->CSR & PWR_CSR_VOSF) {}
}


// Fast wakeup

inline void pwr_enable_fast_wakeup()
{
	PWR->CR |= PWR_CR_FWU;
}

inline void pwr_disable_fast_wakeup()
{
	PWR->CR &= ~PWR_CR_FWU;
}


// Ultralow power mode

inline void pwr_enable_ultralow_power_mode()
{
	PWR->CR |= PWR_CR_ULP;
}

inline void pwr_disable_ultralow_power_mode()
{
	PWR->CR &= ~PWR_CR_ULP;
}


// Disable backup write protection

inline void pwr_disable_backup_write_protection()
{
	PWR->CR |= PWR_CR_DBP;
}

inline void pwr_enable_backup_write_protection()
{
	PWR->CR &= ~PWR_CR_DBP;
}


// PVD level

enum class PVDLevel
{
	_1_9     = 0b000 << 5, // 1.9 V
	_2_1     = 0b001 << 5, // 2.1 V
	_2_3     = 0b010 << 5, // 2.3 V
	_2_5     = 0b011 << 5, // 2.5 V
	_2_7     = 0b100 << 5, // 2.7 V
	_2_9     = 0b101 << 5, // 2.9 V
	_3_1     = 0b110 << 5, // 3.1 V
	External = 0b111 << 5, // External input analog voltage (Compare internally to V REFINT)
};

inline void pwr_set_pvd_level(PVDLevel value)
{
	uint32_t cr = PWR->CR;
	cr &= ~PWR_CR_PLS;
	cr |= (uint32_t)value;
	PWR->CR = cr;
}


// PVD

inline void pwr_enable_pvd()
{
	PWR->CR |= PWR_CR_PVDE;
}

inline void pwr_disable_pvd()
{
	PWR->CR &= ~PWR_CR_PVDE;
}


// standby flag

inline void pwr_clear_standby_flag()
{
	PWR->CR |= PWR_CR_CSBF;
}

inline void pwr_clear_wakeup_flag()
{
	PWR->CR |= PWR_CR_CWUF;
}


// deepsleep mode

inline void pwr_select_deepsleep_mode_as_stop()
{
	PWR->CR &= ~PWR_CR_PDDS;
}

inline void pwr_select_deepsleep_mode_as_standby()
{
	PWR->CR |= PWR_CR_PDDS;
}


// wakeup pin 3

inline void pwr_enable_wakeup_pin3()
{
	PWR->CSR |= PWR_CSR_EWUP3;
}

inline void pwr_disable_wakeup_pin3()
{
	PWR->CSR &= ~PWR_CSR_EWUP3;
}


// wakeup pin 2

inline void pwr_enable_wakeup_pin2()
{
	PWR->CSR |= PWR_CSR_EWUP2;
}

inline void pwr_disable_wakeup_pin2()
{
	PWR->CSR &= ~PWR_CSR_EWUP2;
}


// wakeup pin 1

inline void pwr_enable_wakeup_pin1()
{
	PWR->CSR |= PWR_CSR_EWUP1;
}

inline void pwr_disable_wakeup_pin1()
{
	PWR->CSR &= ~PWR_CSR_EWUP1;
}


// low power run flag

inline bool pwr_get_low_power_run_flag()
{
	return PWR->CSR & PWR_CSR_REGLPF;
}


// Internal voltage reference (V REFINT) ready flag

inline bool pwr_get_v_refint_ready_flag()
{
	return PWR->CSR & PWR_CSR_VREFINTRDYF;
}


// PVD output

inline bool pwr_get_pvd_output()
{
	return PWR->CSR & PWR_CSR_PVDO;
}


// standby flag

inline bool pwr_get_standby_flag()
{
	return PWR->CSR & PWR_CSR_SBF;
}


// wakeup flag

inline bool pwr_get_wakeup_flag()
{
	return PWR->CSR & PWR_CSR_WUF;
}

} // namespace hl
