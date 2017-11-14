#pragma once

#include <stdint.h>

#include "hl_gpio.hpp"
#include "hl_device.hpp"
#include "hl_bb.hpp"

namespace hl {

// fwd.
namespace detailed { template<int N> struct SPI_Helper; }


enum class SPI_BitsCount
{
	_8 = 0,
	_16 = SPI_CR1_DFF
};

enum class SPI_CSMode
{
	Hard = 0,
	Soft = SPI_CR1_SSM,
};

enum class SPI_CPHA
{
	_1_Edge = 0,
	_2_Edge = SPI_CR1_CPHA
};

enum class SPI_CPOL
{
	Low = 0,
	High = SPI_CR1_CPOL
};

enum class SPI_BRPrescaler
{
	_2 = 0x0000,
	_4 = 0x0008,
	_8 = 0x0010,
	_16 = 0x0018,
	_32 = 0x0020,
	_64 = 0x0028,
	_128 = 0x0030,
	_256 = 0x0038
};

template <int N>
class SPI : public Device<detailed::SPI_Helper<N>>
{
public:
	typedef detailed::SPI_Helper<N> Helper;

	static void init(
		const SPI_BitsCount   bits_count,
		const SPI_CSMode      cs_mode,
		const SPI_CPHA        cpha,
		const SPI_CPOL        cpol,
		const SPI_BRPrescaler rate_prescaler)
	{
		static const uint16_t Mode_Master = SPI_CR1_MSTR | SPI_CR1_SSI;

		uint16_t cr1 = HL_UI32REG(cr1_addr) & ~(SPI_CR1_BR|SPI_CR1_DFF|SPI_CR1_SSM|SPI_CR1_CPHA|SPI_CR1_CPOL);

		cr1 |= (
			(uint16_t)rate_prescaler |
			(uint16_t)cs_mode |
			(uint16_t)cpha |
			(uint16_t)cpol |
			(uint16_t)bits_count |
			Mode_Master |
			SPI_CR1_SPE
		);

		// Configure CS output
		if (cs_mode == SPI_CSMode::Hard)
			set_periph_reg_bit<cr2_addr, SPI_CR2_SSOE>();
		else
			clear_periph_reg_bit<cr2_addr, SPI_CR2_SSOE>();

		HL_UI32REG(cr1_addr) = cr1;

		HL_UI32REG(i2scfgr_addr) &= ~SPI_I2SCFGR_I2SMOD;
	}

	template <typename T>
	static T write_and_read(T value)
	{
		HL_UI32REG(dr_addr) = value;
		while (!get_periph_reg_bit<sr_addr, SPI_SR_RXNE>()) {}
		return HL_UI32REG(dr_addr);
	}

	template <typename T>
	static void write(T value)
	{
		HL_UI32REG(dr_addr) = value;
		while (!get_periph_reg_bit<sr_addr, SPI_SR_RXNE>()) {}
		HL_UI32REG(dr_addr);
	}

	static void write16(uint16_t value, unsigned count)
	{
		if (count == 0) return;

		bool old_16_bit_value = get_periph_reg_bit<cr1_addr, SPI_CR1_DFF>();

		if (!old_16_bit_value)
		{
			clear_periph_reg_bit<cr1_addr, SPI_CR1_SPE>(); // disable SPI
			set_periph_reg_bit<cr1_addr, SPI_CR1_DFF>(); // Set 16 bin on
			set_periph_reg_bit<cr1_addr, SPI_CR1_SPE>(); // enable SPI
		}

		while (count != 0)
		{
			HL_UI32REG(dr_addr) = value;
			count--;
			while (!get_periph_reg_bit<sr_addr, SPI_SR_TXE>()) {}
		}

		while (get_periph_reg_bit<sr_addr, SPI_SR_BSY>()) {} // wait for end of transfer

		HL_UI32REG(sr_addr); // clear OVF flag
		HL_UI32REG(dr_addr);

		if (!old_16_bit_value)
		{
			clear_periph_reg_bit<cr1_addr, SPI_CR1_SPE>(); // disable SPI
			clear_periph_reg_bit<cr1_addr, SPI_CR1_DFF>(); // set prev. value for 16 bit
			set_periph_reg_bit<cr1_addr, SPI_CR1_SPE>(); // enable SPI
		}
	}


	template <class CSPin>
	static void cs_high()
	{
		CSPin::on();
	}

	template <class CSPin>
	static void cs_low()
	{
		CSPin::off();
	}

private:
	static constexpr uintptr_t spi_addr = Helper::spi_addr;
	static constexpr uintptr_t sr_addr = spi_addr + offsetof(SPI_TypeDef, SR);
	static constexpr uintptr_t cr1_addr = spi_addr + offsetof(SPI_TypeDef, CR1);
	static constexpr uintptr_t cr2_addr = spi_addr + offsetof(SPI_TypeDef, CR2);
	static constexpr uintptr_t i2scfgr_addr = spi_addr + offsetof(SPI_TypeDef, I2SCFGR);
	static constexpr uintptr_t dr_addr = spi_addr + offsetof(SPI_TypeDef, DR);
};


namespace detailed {

#ifdef RCC_APB2ENR_SPI1EN
template<> struct SPI_Helper<1>
{
	static constexpr uintptr_t spi_addr = SPI1_BASE;
	static constexpr uint32_t RccBit = RCC_APB2ENR_SPI1EN;
	static constexpr uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2ENR);
	static constexpr uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2RSTR);
};
#endif

#ifdef RCC_APB1ENR_SPI2EN
template<> struct SPI_Helper<2>
{
	static constexpr uintptr_t spi_addr = SPI2_BASE;
	static constexpr uint32_t RccBit = RCC_APB1ENR_SPI2EN;
	static constexpr uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, APB1ENR);
	static constexpr uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, APB1RSTR);
};
#endif

#ifdef RCC_APB1ENR_SPI3EN
template<> struct SPI_Helper<3>
{
	static constexpr uintptr_t spi = SPI3_BASE;
	static constexpr uint32_t RccBit = RCC_APB1ENR_SPI3EN;
	static constexpr uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, APB1ENR);
	static constexpr uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, APB1RSTR);
};
#endif



} // namespace detailed


} // namespace hl
