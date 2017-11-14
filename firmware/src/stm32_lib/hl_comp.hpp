#pragma once

#include <stdint.h>
#include "hl_device.hpp"
#include "hl_gpio.hpp"

namespace hl {

#if defined (HL_STM32L1XX)

// fwd.
namespace detailed { struct CompDevice; }

enum class Comp2Output
{
	TIM2_InputCapture4   = 0 << 21,
	TIM2_OCREF_CLR       = 1 << 21,
	TIM3_InputCapture4   = 2 << 21,
	TIM3_OCREF_CLR       = 3 << 21,
	TIM4_InputCapture4   = 4 << 21,
	TIM4_OCREF_CLR       = 5 << 21,
	TIM10_Input_Capture1 = 6 << 21,
	NoRedirection        = 7 << 21,
};

enum class Comp2InvInput
{
	NoSelection = 0b000 << 18,
	Ext_IO_PB3  = 0b001 << 18,
	_1_RefInt   = 0b010 << 18,
	_3_4_RefInt = 0b011 << 18,
	_1_2_RefInt = 0b100 << 18,
	_1_4_RefInt = 0b101 << 18,
	DACOut1     = 0b110 << 18,
	DACOut2     = 0b111 << 18,
};

enum class Comp2Speed
{
	Slow = 0b0 << 12,
	Fast = 0b1 << 12,
};

class Comp : public Device<detailed::CompDevice>
{
public:
	static void enable_suspend_timer_mode()
	{
		clear_periph_reg_bit<COMP_CSR, COMP_CSR_TSUSP>();
	}

	static void disable_suspend_timer_mode()
	{
		set_periph_reg_bit<COMP_CSR, COMP_CSR_TSUSP>();
	}

	static bool get_acquisition_interrupt_flag()
	{
		return get_periph_reg_bit<COMP_CSR, COMP_CSR_CAIF>();
	}

	static void enable_acquisition_interrupt()
	{
		set_periph_reg_bit<COMP_CSR, COMP_CSR_CAIE>();
	}

	static void disable_acquisition_interrupt()
	{
		clear_periph_reg_bit<COMP_CSR, COMP_CSR_CAIE>();
	}

	static void select_pc3_as_rerouted_adc_chan()
	{
		set_periph_reg_bit<COMP_CSR, COMP_CSR_RCH13>();
	}

	static void select_pc3_as_slow_adc_chan()
	{
		clear_periph_reg_bit<COMP_CSR, COMP_CSR_RCH13>();
	}

	static void select_pb0_as_fast_adc_chan()
	{
		set_periph_reg_bit<COMP_CSR, COMP_CSR_FCH8>();
	}

	static void select_pb0_as_slow_adc_chan()
	{
		clear_periph_reg_bit<COMP_CSR, COMP_CSR_FCH8>();
	}

	static void select_pa3_as_fast_adc_chan()
	{
		set_periph_reg_bit<COMP_CSR, COMP_CSR_FCH3>();
	}

	static void select_pa3_as_slow_adc_chan()
	{
		clear_periph_reg_bit<COMP_CSR, COMP_CSR_FCH3>();
	}

	static void select_comp2_output(Comp2Output value)
	{
		set_value_by_mask<uint32_t>(COMP_CSR, COMP_CSR_OUTSEL, (uint32_t)value);
	}

	static void select_comp2_inv_input(Comp2InvInput value)
	{
		set_value_by_mask<uint32_t>(COMP_CSR, COMP_CSR_INSEL, (uint32_t)value);
	}

	static void enable_window_mode()
	{
		set_periph_reg_bit<COMP_CSR, COMP_CSR_WNDWE>();
	}

	static void disable_window_mode()
	{
		clear_periph_reg_bit<COMP_CSR, COMP_CSR_WNDWE>();
	}

	static void enable_refint_output()
	{
		set_periph_reg_bit<COMP_CSR, COMP_CSR_VREFOUTEN>();
	}

	static void disable_refint_output()
	{
		clear_periph_reg_bit<COMP_CSR, COMP_CSR_VREFOUTEN>();
	}

	// CMP2OUT ???

	static void set_comp2_speed(Comp2Speed speed)
	{
		set_value_by_mask<uint32_t>(COMP_CSR, COMP_CSR_SPEED, (uint32_t)speed);
	}

	static void open_comp1_analog_switch()
	{
		clear_periph_reg_bit<COMP_CSR, COMP_CSR_SW1>();
	}

	static void close_comp1_analog_switch()
	{
		set_periph_reg_bit<COMP_CSR, COMP_CSR_SW1>();
	}

	static void enable_comp1()
	{
		set_periph_reg_bit<COMP_CSR, COMP_CSR_CMP1EN>();
	}

	static void disable_comp1()
	{
		clear_periph_reg_bit<COMP_CSR, COMP_CSR_CMP1EN>();
	}

	static void enable_400k_pd_resistor()
	{
		set_periph_reg_bit<COMP_CSR, COMP_CSR_400KPD>();
	}

	static void disable_400k_pd_resistor()
	{
		clear_periph_reg_bit<COMP_CSR, COMP_CSR_400KPD>();
	}

	static void enable_10k_pd_resistor()
	{
		set_periph_reg_bit<COMP_CSR, COMP_CSR_10KPD>();
	}

	static void disable_10k_pd_resistor()
	{
		clear_periph_reg_bit<COMP_CSR, COMP_CSR_10KPD>();
	}

	static void enable_400k_pu_resistor()
	{
		set_periph_reg_bit<COMP_CSR, COMP_CSR_400KPU>();
	}

	static void disable_400k_pu_resistor()
	{
		clear_periph_reg_bit<COMP_CSR, COMP_CSR_400KPU>();
	}

	static void enable_10k_pu_resistor()
	{
		set_periph_reg_bit<COMP_CSR, COMP_CSR_10KPU>();
	}

	static void disable_10k_pu_resistor()
	{
		clear_periph_reg_bit<COMP_CSR, COMP_CSR_10KPU>();
	}

private:
	static constexpr uintptr_t COMP_CSR = COMP_BASE + offsetof(COMP_TypeDef, CSR);
};

namespace detailed {

struct CompDevice
{
	static constexpr uint32_t RccBit = RCC_APB1ENR_COMPEN;
	static constexpr uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, APB1ENR);
	static constexpr uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, APB1RSTR);
};

} // namespace detailed


///////////////////////////////////////////////////////////////////////////////
#elif defined (HL_STM32F3XX)

namespace detailed {
template <unsigned N> struct CompHelper;
template <unsigned N> struct CompPinsHelper;
}


enum class CompBlankingSource
{
	No       = 0b000 << 18, // No blanking
	TIM1_OC5 = 0b001 << 18, // TIM1 OC5 selected as blanking source
	TIM2_OC3 = 0b010 << 18, // TIM2 OC3 selected as blanking source
	TIM3_OC3 = 0b011 << 18, // TIM3 OC3 selected as blanking source
};

enum class CompHysteresis
{
	No     = 0b00 << 16,
	Low    = 0b01 << 16,
	Medium = 0b10 << 16,
	High   = 0b11 << 16,
};

enum class CompOutPol
{
	Normal   = 0b0 << 15,
	Inverted = 0b1 << 15,
};

enum class CompOut
{
	No                       = 0b0000 << 10,
	Timer1BreakInput         = 0b0001 << 10,
	Timer1BreakInput2        = 0b0010 << 10,
	Timer8BreakInput1        = 0b0011 << 10,
	Timer8BreakInput2        = 0b0100 << 10,
	Timer1and8BreakInput2    = 0b0101 << 10,
	Timer1OCrefClearInput    = 0b0110 << 10,
	Timer1InputCapture1      = 0b0111 << 10,
	Timer2InputCapture4      = 0b1000 << 10,
	Timer2OCrefClearInput    = 0b1001 << 10,
	Timer3InputCapture1      = 0b1010 << 10,
	Timer3OCrefclearInput    = 0b1011 << 10,
	Timer20BreakInput1       = 0b1100 << 10,
	Timer20BreakInput2       = 0b1101 << 10,
	Timer1or8or20BreakInput2 = 0b1110 << 10,
};

enum class CompInput
{
	Comp2_PA7  = 0b0 << 7,
	Comp2_PA3  = 0b1 << 7,
	Comp3_PB14 = 0b0 << 7,
	Comp3_PD14 = 0b1 << 7,
	Comp4_PB0  = 0b0 << 7,
	Comp4_PE7  = 0b1 << 7,
	Comp5_PD12 = 0b0 << 7,
	Comp5_PB13 = 0b1 << 7,
	Comp6_PD11 = 0b0 << 7,
	Comp6_PB11 = 0b1 << 7,
	Comp7_PA0  = 0b0 << 7,
	Comp7_PC1  = 0b1 << 7,
};

enum class CompInvInput
{
	_1_4_Vrefint = 0b0000 << 4,
	_1_2_Vrefint = 0b0001 << 4,
	_3_4_Vrefint = 0b0010 << 4,
	Vrefint      = 0b0011 << 4,
	PA4orDAC1    = 0b0100 << 4,
	PA5orDAC2    = 0b0101 << 4,
	Comp1_PA0    = 0b0110 << 4,
	Comp2_PA2    = 0b0110 << 4,
	Comp3_PD15   = 0b0110 << 4,
	Comp3_PB12   = 0b0111 << 4,
	Comp4_PE8    = 0b0110 << 4,
	Comp4_PB2    = 0b0111 << 4,
	Comp5_PD13   = 0b0110 << 4,
	Comp5_PB10   = 0b0111 << 4,
	Comp6_PD10   = 0b0110 << 4,
	Comp6_PB15   = 0b0111 << 4,
	Comp7_PC0    = 0b0110 << 4,
	DAC2Ch1      = 0b1    << 22,
};

template <unsigned N>
class Comp
{
public:
	static constexpr IRQn_Type IRQn = detailed::CompHelper<N>::IRQn;
	static constexpr uint8_t EXTILine = detailed::CompHelper<N>::EXTILine;

	static void lock()
	{
		set_periph_reg_bit<COMP_CSR, COMP1_CSR_COMP1LOCK>();
	}

	static bool is_locked()
	{
		return get_periph_reg_bit<COMP_CSR, COMP1_CSR_COMP1LOCK>();
	}

	static bool get_out()
	{
		return get_periph_reg_bit<COMP_CSR, COMP1_CSR_COMP1OUT>();
	}

	static void set_blanking_source(CompBlankingSource source)
	{
		set_value_by_mask<uint32_t>(COMP_CSR, COMP1_CSR_COMP1BLANKING, (uint32_t)source);
	}

	static void set_hysteresis(CompHysteresis hysteresis)
	{
		set_value_by_mask<uint32_t>(COMP_CSR, COMP1_CSR_COMP1HYST, (uint32_t)hysteresis);
	}

	static void set_out_polarity(CompOutPol polarity)
	{
		set_value_by_mask<uint32_t>(COMP_CSR, COMP1_CSR_COMP1POL, (uint32_t)polarity);
	}

	static void set_out(CompOut out)
	{
		set_value_by_mask<uint32_t>(COMP_CSR, COMP1_CSR_COMP1OUTSEL, (uint32_t)out);
	}

	static void enable_window_mode()
	{
		set_periph_reg_bit<COMP_CSR, COMP2_CSR_COMP2WNDWEN>();
	}

	static void disable_window_mode()
	{
		clear_periph_reg_bit<COMP_CSR, COMP2_CSR_COMP2WNDWEN>();
	}

	static void set_input(CompInput input)
	{
		set_value_by_mask<uint32_t>(COMP_CSR, COMP1_CSR_COMP1NONINSEL, (uint32_t)input);
	}

	static void set_inv_input(CompInvInput input)
	{
		set_value_by_mask<uint32_t>(COMP_CSR, COMP_CSR_COMPINSEL, (uint32_t)input);
	}

	static void enable()
	{
		set_periph_reg_bit<COMP_CSR, COMP1_CSR_COMP1EN>();
	}

	static void disable()
	{
		clear_periph_reg_bit<COMP_CSR, COMP1_CSR_COMP1EN>();
	}

	static bool is_enabled()
	{
		return get_periph_reg_bit<COMP_CSR, COMP1_CSR_COMP1EN>();
	}

private:
	static constexpr uintptr_t comp = detailed::CompHelper<N>::comp;
	static constexpr uintptr_t COMP_CSR = comp + offsetof(COMP_TypeDef, CSR);
	static constexpr uint32_t COMP_CSR_COMPINSEL = detailed::CompHelper<N>::COMP_CSR_COMPINSEL;
};

///////////////////////////////////////////////////////////////////////////////

#ifdef COMP1
namespace detailed { template <> class CompHelper<1>
{
	static constexpr uintptr_t comp = COMP1_BASE;
	static constexpr uint32_t COMP_CSR_COMPINSEL = COMP1_CSR_COMP1INSEL;
	static constexpr IRQn_Type IRQn = COMP1_2_3_IRQn;
	static constexpr uint8_t EXTILine = 21;
};}
typedef Comp<1> Comp1;
#endif

#ifdef COMP2
namespace detailed { template <> struct CompHelper<2>
{
	static constexpr uintptr_t comp = COMP2_BASE;
	static constexpr uint32_t COMP_CSR_COMPINSEL = COMP2_CSR_COMP2INSEL;
	static constexpr IRQn_Type IRQn = COMP1_2_3_IRQn;
	static constexpr uint8_t EXTILine = 22;
};}
typedef Comp<2> Comp2;
#endif

#ifdef COMP3
namespace detailed { template <> class CompHelper<3>
{
	static constexpr uintptr_t comp = COMP3_BASE;
	static constexpr uint32_t COMP_CSR_COMPINSEL = COMP3_CSR_COMP3INSEL;
	static constexpr IRQn_Type IRQn = COMP1_2_3_IRQn;
	static constexpr uint8_t EXTILine = 29;
};}
typedef Comp<3> Comp3;
#endif

#ifdef COMP4
namespace detailed { template <> class CompHelper<4>
{
	static constexpr uintptr_t comp = COMP4_BASE;
	static constexpr uint32_t COMP_CSR_COMPINSEL = COMP4_CSR_COMP4INSEL;
	static constexpr IRQn_Type IRQn = COMP4_5_6_IRQn;
	static constexpr uint8_t EXTILine = 30;
};}
typedef Comp<4> Comp4;
#endif

#ifdef COMP5
namespace detailed { template <> class CompHelper<5>
{
	static constexpr uintptr_t comp = COMP5_BASE;
	static constexpr uint32_t COMP_CSR_COMPINSEL = COMP5_CSR_COMP5INSEL;
	static constexpr IRQn_Type IRQn = COMP4_5_6_IRQn;
	static constexpr uint8_t EXTILine = 31;
};}
typedef Comp<5> Comp5;
#endif

#ifdef COMP6
namespace detailed { template <> class CompHelper<6>
{
	static constexpr uintptr_t comp = COMP6_BASE;
	static constexpr uint32_t COMP_CSR_COMPINSEL = COMP6_CSR_COMP6INSEL;
	static constexpr IRQn_Type IRQn = COMP4_5_6_IRQn;
	static constexpr uint8_t EXTILine = 32;
};}
typedef Comp<6> Comp6;
#endif

#ifdef COMP7
namespace detailed { template <> class CompHelper<7>
{
	static constexpr uintptr_t comp = COMP7_BASE;
	static constexpr uint32_t COMP_CSR_COMPINSEL = COMP7_CSR_COMP7INSEL;
	static constexpr IRQn_Type IRQn = COMP7_IRQn;
	static constexpr uint8_t EXTILine = 33;
};}
typedef Comp<7> Comp7;
#endif

#endif
} // namespace hl
