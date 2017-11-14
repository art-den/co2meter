#pragma once

#include <stdint.h>
#include "hl_device.hpp"

namespace hl {

namespace detailed {
	struct DACHelper;
	template <unsigned N> struct DACChannelHelper;
}

enum class DACAmpl
{
	_1,
	_3,
	_7,
	_15,
	_31,
	_63,
	_127,
	_255,
	_511,
	_1023,
	_2047,
	_4095,
};

enum class DACWaveMode
{
	Disabled = 0b00,
	Noise    = 0b01,
	Triangle = 0b10,
};

enum class DACTrigger
{
	Timer6        = 0b000,
	Timer7        = 0b010,
	Timer9        = 0b011,
	Timer2        = 0b100,
	Timer4        = 0b101,
	ExternalLine9 = 0b110,
	Software      = 0b111,
};

class Dac : public Device<detailed::DACHelper>
{
public:
};

template <unsigned N>
class DacChannel
{
public:
	// config

#if defined (DAC_CR_DMAUDRIE1) && defined(DAC_CR_DMAUDRIE2)
	static void endble_dma_underrun_interrupt()
	{
		set_periph_reg_bit<CR_ADDR, Helper::DMAUDRIE>();
	}

	static void disable_dma_underrun_interrupt()
	{
		clear_periph_reg_bit<CR_ADDR, Helper::DMAUDRIE>();
	}
#endif

	static void endble_dma()
	{
		set_periph_reg_bit<CR_ADDR, Helper::DMAEN>();
	}

	static void disable_dma()
	{
		clear_periph_reg_bit<CR_ADDR, Helper::DMAEN>();
	}

	static void set_amplitude(DACAmpl ampl)
	{
		set_value_by_mask(DAC->CR, Helper::MAMP_MASK, (uint32_t)ampl << Helper::MAMP_SHIFT);
	}

	static void set_wave_mode(DACWaveMode mode)
	{
		set_value_by_mask(DAC->CR, Helper::WAVE_MASK, (uint32_t)mode << Helper::WAVE_SHIFT);
	}

	static void set_trigger(DACTrigger trigger)
	{
		set_value_by_mask(DAC->CR, Helper::TSEL_MASK, (uint32_t)trigger << Helper::TSEL_SHIFT);
	}

	static void enable_trigger()
	{
		set_periph_reg_bit<CR_ADDR, Helper::TEN>();
	}

	static void disable_trigger()
	{
		clear_periph_reg_bit<CR_ADDR, Helper::TEN>();
	}

	static void enable_output_buffer()
	{
		clear_periph_reg_bit<CR_ADDR, Helper::BOFF>();
	}

	static void disable_output_buffer()
	{
		set_periph_reg_bit<CR_ADDR, Helper::BOFF>();
	}

	static void enable()
	{
		set_periph_reg_bit<CR_ADDR, Helper::EN>();
	}

	static void disable()
	{
		clear_periph_reg_bit<CR_ADDR, Helper::EN>();
	}

	static void set_software_trigger()
	{
		set_periph_reg_bit<SWTRIGR_ADDR, Helper::SWTRIG>();
	}

	static void reset_software_trigger()
	{
		clear_periph_reg_bit<SWTRIGR_ADDR, Helper::SWTRIG>();
	}

	static void set_right_aligned_value12(uint16_t value)
	{
		HL_UI32REG(Helper::data_hold_reg_12r) = value;
	}

	static void set_left_aligned_value12(uint16_t value)
	{
		HL_UI32REG(Helper::data_hold_reg_12l) = value;
	}

	static void set_value8(uint8_t value)
	{
		HL_UI32REG(Helper::data_hold_reg_8r) = value;
	}

	static void set_value12r(uint16_t value)
	{
		HL_UI32REG(Helper::data_hold_reg_12r) = value;
	}

	static uint16_t get_data()
	{
		return HL_UI32REG(Helper::data_reg) & 0xFFF;
	}

private:
	typedef detailed::DACChannelHelper<N> Helper;
	static constexpr uintptr_t CR_ADDR = DAC_BASE + offsetof(DAC_TypeDef, CR);
	static constexpr uintptr_t SWTRIGR_ADDR = DAC_BASE + offsetof(DAC_TypeDef, SWTRIGR);
};

typedef DacChannel<1> DacChannel1;
typedef DacChannel<2> DacChannel2;

namespace detailed {

struct DACHelper
{
#if defined (HL_STM32L1XX)
	static constexpr uint32_t RccBit = RCC_APB1ENR_DACEN;
#elif defined (HL_STM32F3XX)
	static constexpr uint32_t RccBit = RCC_APB1ENR_DAC1EN;
#endif
	static constexpr uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, APB1ENR);
	static constexpr uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, APB1RSTR);
};

template <> struct DACChannelHelper<1>
{
#ifdef DAC_CR_DMAUDRIE1
	static constexpr uint32_t DMAUDRIE   = DAC_CR_DMAUDRIE1;
#endif
	static constexpr uint32_t DMAEN      = DAC_CR_DMAEN1;
	static constexpr uint32_t MAMP_MASK  = DAC_CR_MAMP1;
	static constexpr uint32_t MAMP_SHIFT = 8;
	static constexpr uint32_t WAVE_MASK  = DAC_CR_WAVE1;
	static constexpr uint32_t WAVE_SHIFT = 6;
	static constexpr uint32_t TSEL_MASK  = DAC_CR_TSEL1;
	static constexpr uint32_t TSEL_SHIFT = 3;
	static constexpr uint32_t TEN        = DAC_CR_TEN1;
	static constexpr uint32_t BOFF       = DAC_CR_BOFF1;
	static constexpr uint32_t EN         = DAC_CR_EN1;
	static constexpr uint32_t SWTRIG     = DAC_SWTRIGR_SWTRIG1;

	static constexpr uintptr_t data_hold_reg_12r = DAC_BASE + offsetof(DAC_TypeDef, DHR12R1);
	static constexpr uintptr_t data_hold_reg_12l = DAC_BASE + offsetof(DAC_TypeDef, DHR12L1);
	static constexpr uintptr_t data_hold_reg_8r = DAC_BASE + offsetof(DAC_TypeDef, DHR8R1);
	static constexpr uintptr_t data_reg = DAC_BASE + offsetof(DAC_TypeDef, DOR1);
};

template <> struct DACChannelHelper<2>
{
#ifdef DAC_CR_DMAUDRIE2
	static constexpr uint32_t DMAUDRIE   = DAC_CR_DMAUDRIE2;
#endif
	static constexpr uint32_t DMAEN      = DAC_CR_DMAEN2;
	static constexpr uint32_t MAMP_MASK  = DAC_CR_MAMP2;
	static constexpr uint32_t MAMP_SHIFT = 24;
	static constexpr uint32_t WAVE_MASK  = DAC_CR_WAVE2;
	static constexpr uint32_t WAVE_SHIFT = 22;
	static constexpr uint32_t TSEL_MASK  = DAC_CR_TSEL2;
	static constexpr uint32_t TSEL_SHIFT = 19;
	static constexpr uint32_t TEN        = DAC_CR_TEN2;
	static constexpr uint32_t BOFF       = DAC_CR_BOFF2;
	static constexpr uint32_t EN         = DAC_CR_EN2;
	static constexpr uint32_t SWTRIG     = DAC_SWTRIGR_SWTRIG2;

	static constexpr uintptr_t data_hold_reg_12r = DAC_BASE + offsetof(DAC_TypeDef, DHR12R2);
	static constexpr uintptr_t data_hold_reg_12l = DAC_BASE + offsetof(DAC_TypeDef, DHR12L2);
	static constexpr uintptr_t data_hold_reg_8r = DAC_BASE + offsetof(DAC_TypeDef, DHR8R2);
	static constexpr uintptr_t data_reg = DAC_BASE + offsetof(DAC_TypeDef, DOR2);
};


} // namespace detailed
} // namespace hl
