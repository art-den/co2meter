#pragma once

#include <stm32l1xx.h>
#include <stdint.h>

#include "hl_gpio.hpp"

namespace hl {

#if defined (HL_STM32L1XX)


// fwd.
namespace detailed { template <unsigned N> struct OpAmpHelper; }


template <unsigned N>
class OpAmp
{
public:
	using PosInPin = typename detailed::OpAmpHelper<N>::PosInPin;
	using NegInPin = typename detailed::OpAmpHelper<N>::NegInPin;
	using OutPin = typename detailed::OpAmpHelper<N>::OutPin;


	// calibration output

	static bool get_calibration_output()
	{
		return OPAMP->CSR & Helper::OPAMP_CSR_OPA_CALOUT;
	}


	// ana switch

	static void open_ana_switch()
	{
		OPAMP->CSR &= ~Helper::OPAMP_CSR_ANAWSEL;
	}

	static void close_ana_switch()
	{
		OPAMP->CSR |= Helper::OPAMP_CSR_ANAWSEL;
	}

	static bool get_ana_switch()
	{
		return OPAMP->CSR & Helper::OPAMP_CSR_ANAWSEL;
	}


	// low power mode

	static void enable_low_power_mode()
	{
		OPAMP->CSR |= Helper::OPAMP_CSR_OPA_LPM;
	}

	static void disable_low_power_mode()
	{
		OPAMP->CSR &= ~Helper::OPAMP_CSR_OPA_LPM;
	}

	static bool get_low_power_mode()
	{
		return OPAMP->CSR & Helper::OPAMP_CSR_OPA_LPM;
	}


	// offset calibration for N diff

	static void set_offset_calibration_for_n_diff_on()
	{
		OPAMP->CSR |= Helper::OPAMP_CSR_OPA_CAL_H;
	}

	static void set_offset_calibration_for_n_diff_off()
	{
		OPAMP->CSR &= ~Helper::OPAMP_CSR_OPA_CAL_H;
	}

	static bool get_offset_calibration_for_n_diff()
	{
		return OPAMP->CSR & Helper::OPAMP_CSR_OPA_CAL_H;
	}


	// offset calibration for P diff

	static void set_offset_calibration_for_p_diff_on()
	{
		OPAMP->CSR |= Helper::OPAMP_CSR_OPA_CAL_L;
	}

	static void set_offset_calibration_for_p_diff_off()
	{
		OPAMP->CSR &= ~Helper::OPAMP_CSR_OPA_CAL_L;
	}

	static bool get_offset_calibration_for_p_diff()
	{
		return OPAMP->CSR & Helper::OPAMP_CSR_OPA_CAL_L;
	}


	// switch 7

	static void open_switch_7()
	{
		OPAMP->CSR &= ~Helper::OPAMP_CSR_S7SEL;
	}

	static void close_switch_7()
	{
		OPAMP->CSR |= Helper::OPAMP_CSR_S7SEL;
	}

	static bool is_switch_7_closed()
	{
		return OPAMP->CSR & Helper::OPAMP_CSR_S7SEL;
	}


	// switch 6

	static void open_switch_6()
	{
		OPAMP->CSR &= ~Helper::OPAMP_CSR_S6SEL;
	}

	static void close_switch_6()
	{
		OPAMP->CSR |= Helper::OPAMP_CSR_S6SEL;
	}

	static bool is_switch_6_closed()
	{
		return OPAMP->CSR & Helper::OPAMP_CSR_S6SEL;
	}


	// switch 5

	static void open_switch_5()
	{
		OPAMP->CSR &= ~Helper::OPAMP_CSR_S5SEL;
	}

	static void close_switch_5()
	{
		OPAMP->CSR |= Helper::OPAMP_CSR_S5SEL;
	}

	static bool is_switch_5_closed()
	{
		return OPAMP->CSR & Helper::OPAMP_CSR_S5SEL;
	}


	// switch 4

	static void open_switch_4()
	{
		OPAMP->CSR &= ~Helper::OPAMP_CSR_S4SEL;
	}

	static void close_switch_4()
	{
		OPAMP->CSR |= Helper::OPAMP_CSR_S4SEL;
	}

	static bool is_switch_4_closed()
	{
		return OPAMP->CSR & Helper::OPAMP_CSR_S4SEL;
	}

	// switch 3

	static void open_switch_3()
	{
		OPAMP->CSR &= ~Helper::OPAMP_CSR_S3SEL;
	}

	static void close_switch_3()
	{
		OPAMP->CSR |= Helper::OPAMP_CSR_S3SEL;
	}

	static bool is_switch_3_closed()
	{
		return OPAMP->CSR & Helper::OPAMP_CSR_S3SEL;
	}


	// power

	static void power_down()
	{
		OPAMP->CSR |= Helper::OPAMP_CSR_OPA_PD;
	}

	static void power_up()
	{
		OPAMP->CSR &= ~Helper::OPAMP_CSR_OPA_PD;
	}

	static bool is_power_down()
	{
		return OPAMP->CSR & Helper::OPAMP_CSR_OPA_PD;
	}


private:
	typedef detailed::OpAmpHelper<N> Helper;
};

inline void opamps_set_low_power_range()
{
	OPAMP->CSR &= ~OPAMP_CSR_AOP_RANGE;
}

inline void opamps_set_high_power_range()
{
	OPAMP->CSR |= OPAMP_CSR_AOP_RANGE;
}


namespace detailed {

template <> struct OpAmpHelper<1>
{
	static constexpr uint32_t OPAMP_CSR_OPA_CALOUT = OPAMP_CSR_OPA1CALOUT;
	static constexpr uint32_t OPAMP_CSR_ANAWSEL    = OPAMP_CSR_ANAWSEL1;
	static constexpr uint32_t OPAMP_CSR_OPA_LPM    = OPAMP_CSR_OPA1LPM;
	static constexpr uint32_t OPAMP_CSR_OPA_CAL_H  = OPAMP_CSR_OPA1CAL_H;
	static constexpr uint32_t OPAMP_CSR_OPA_CAL_L  = OPAMP_CSR_OPA1CAL_L;
	static constexpr uint32_t OPAMP_CSR_S6SEL      = OPAMP_CSR_S6SEL1;
	static constexpr uint32_t OPAMP_CSR_S5SEL      = OPAMP_CSR_S5SEL1;
	static constexpr uint32_t OPAMP_CSR_S4SEL      = OPAMP_CSR_S4SEL1;
	static constexpr uint32_t OPAMP_CSR_S3SEL      = OPAMP_CSR_S3SEL1;
	static constexpr uint32_t OPAMP_CSR_OPA_PD     = OPAMP_CSR_OPA1PD;

	typedef PA1 PosInPin;
	typedef PA2 NegInPin;
	typedef PA3 OutPin;
};


template <> struct OpAmpHelper<2>
{
	static constexpr uint32_t OPAMP_CSR_OPA_CALOUT = OPAMP_CSR_OPA2CALOUT;
	static constexpr uint32_t OPAMP_CSR_ANAWSEL    = OPAMP_CSR_ANAWSEL2;
	static constexpr uint32_t OPAMP_CSR_OPA_LPM    = OPAMP_CSR_OPA2LPM;
	static constexpr uint32_t OPAMP_CSR_OPA_CAL_H  = OPAMP_CSR_OPA2CAL_H;
	static constexpr uint32_t OPAMP_CSR_OPA_CAL_L  = OPAMP_CSR_OPA2CAL_L;
	static constexpr uint32_t OPAMP_CSR_S7SEL      = OPAMP_CSR_S7SEL2;
	static constexpr uint32_t OPAMP_CSR_S6SEL      = OPAMP_CSR_S6SEL2;
	static constexpr uint32_t OPAMP_CSR_S5SEL      = OPAMP_CSR_S5SEL2;
	static constexpr uint32_t OPAMP_CSR_S4SEL      = OPAMP_CSR_S4SEL2;
	static constexpr uint32_t OPAMP_CSR_S3SEL      = OPAMP_CSR_S3SEL2;
	static constexpr uint32_t OPAMP_CSR_OPA_PD     = OPAMP_CSR_OPA2PD;

	typedef PA6 PosInPin;
	typedef PA7 NegInPin;
	typedef PB0 OutPin;
};


template <> struct OpAmpHelper<3>
{
	static constexpr uint32_t OPAMP_CSR_OPA_CALOUT = OPAMP_CSR_OPA3CALOUT;
	static constexpr uint32_t OPAMP_CSR_ANAWSEL    = OPAMP_CSR_ANAWSEL3;
	static constexpr uint32_t OPAMP_CSR_OPA_LPM    = OPAMP_CSR_OPA3LPM;
	static constexpr uint32_t OPAMP_CSR_OPA_CAL_H  = OPAMP_CSR_OPA3CAL_H;
	static constexpr uint32_t OPAMP_CSR_OPA_CAL_L  = OPAMP_CSR_OPA3CAL_L;
	static constexpr uint32_t OPAMP_CSR_S6SEL      = OPAMP_CSR_S6SEL3;
	static constexpr uint32_t OPAMP_CSR_S5SEL      = OPAMP_CSR_S5SEL3;
	static constexpr uint32_t OPAMP_CSR_S4SEL      = OPAMP_CSR_S4SEL3;
	static constexpr uint32_t OPAMP_CSR_S3SEL      = OPAMP_CSR_S3SEL3;
	static constexpr uint32_t OPAMP_CSR_OPA_PD     = OPAMP_CSR_OPA3PD;

	typedef PC1 PosInPin;
	typedef PC2 NegInPin;
	typedef PC3 OutPin;
};

} // namespace detailed

#endif

} // namespace hl
