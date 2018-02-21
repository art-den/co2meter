#pragma once

#include <stdint.h>
#include "hl_device.hpp"

namespace hl {

// fwd.
namespace detailed {
	template <unsigned> struct DMAHelper;
	template <unsigned> struct DMAChanHelper;
	template <unsigned, unsigned> struct DMAChanHelperEx;
}

enum class DMAChanPriority
{
	Low      = 0b00 << 12,
	Medium   = 0b01 << 12,
	High     = 0b10 << 12,
	VeryHigh = 0b11 << 12,
};

enum class DMADataSize
{
	_8bit,
	_16bit,
	_32bit
};

enum class DMADirection
{
	PeriphToMem = 0 << 4,
	MemToPeriph = 1 << 4,
};

template <unsigned N>
class DMA_ : public Device<detailed::DMAHelper<N>>
{
public:

private:
	static constexpr DMA_TypeDef *dma = detailed::DMAHelper<N>::dma;
};

template <unsigned DmaN, unsigned ChanN>
class DMAChannel
{
public:
	typedef detailed::DMAChanHelper<ChanN> ChanHelper;
	typedef detailed::DMAChanHelperEx<DmaN, ChanN> ChanHelperEx;

	static constexpr IRQn_Type IRQn = ChanHelperEx::IRQn;

	// status flags

	static bool get_transfer_error_flag()
	{
		return get_periph_reg_bit<DMA_ISR, ChanHelper::TransferErrorFlag>();
	}

	static void clear_transfer_error_flag()
	{
		HL_UI32REG(DMA_IFCR) = ChanHelper::TransferErrorFlag;
	}

	static bool get_half_transfer_flag()
	{
		return get_periph_reg_bit<DMA_ISR, ChanHelper::HalfTransferFlag>();
	}

	static void clear_half_transfer_flag()
	{
		HL_UI32REG(DMA_IFCR) = ChanHelper::HalfTransferFlag;
	}

	static bool get_transfer_complete_flag()
	{
		return get_periph_reg_bit<DMA_ISR, ChanHelper::TransferCompleteFlag>();
	}

	static void clear_transfer_complete_flag()
	{
		HL_UI32REG(DMA_IFCR) = ChanHelper::TransferCompleteFlag;
	}

	static bool get_global_interrupt_flag()
	{
		return get_periph_reg_bit<DMA_ISR, ChanHelper::GlobalInterruptFlag>();
	}

	static void clear_global_interrupt_flag()
	{
		HL_UI32REG(DMA_IFCR) = ChanHelper::GlobalInterruptFlag;
	}

	// control

	static void enable_mem2mem_mode()
	{
		set_periph_reg_bit<CHAN_CCR, DMA_CCR_MEM2MEM>();
	}

	static void disable_mem2mem_mode()
	{
		clear_periph_reg_bit<CHAN_CCR, DMA_CCR_MEM2MEM>();
	}

	static void set_priority(DMAChanPriority priority)
	{
		set_value_by_mask<uint32_t>(CHAN_CCR, DMA_CCR_PL, (uint32_t)priority);
	}

	static void set_memory_size(DMADataSize size)
	{
		set_value_by_mask<uint32_t>(CHAN_CCR, DMA_CCR_MSIZE, (uint32_t)size << 10);
	}

	static void set_peripheral_size(DMADataSize size)
	{
		set_value_by_mask<uint32_t>(CHAN_CCR, DMA_CCR_PSIZE, (uint32_t)size << 8);
	}

	static void enable_memory_increment()
	{
		set_periph_reg_bit<CHAN_CCR, DMA_CCR_MINC>();
	}

	static void disable_memory_increment()
	{
		clear_periph_reg_bit<CHAN_CCR, DMA_CCR_MINC>();
	}

	static void enable_peripheral_increment()
	{
		set_periph_reg_bit<CHAN_CCR, DMA_CCR_PINC>();
	}

	static void disable_peripheral_increment()
	{
		clear_periph_reg_bit<CHAN_CCR, DMA_CCR_PINC>();
	}

	static void enable_circular_mode()
	{
		set_periph_reg_bit<CHAN_CCR, DMA_CCR_CIRC>();
	}

	static void disable_circular_mode()
	{
		clear_periph_reg_bit<CHAN_CCR, DMA_CCR_CIRC>();
	}

	static void set_direction(DMADirection direction)
	{
		set_value_by_mask<uint32_t>(CHAN_CCR, DMA_CCR_DIR, (uint32_t)direction);
	}

	static void enable_transfer_error_interrupt()
	{
		set_periph_reg_bit<CHAN_CCR, DMA_CCR_TEIE>();
	}

	static void disable_transfer_error_interrupt()
	{
		clear_periph_reg_bit<CHAN_CCR, DMA_CCR_TEIE>();
	}

	static void enable_half_transfer_interrupt()
	{
		set_periph_reg_bit<CHAN_CCR, DMA_CCR_HTIE>();
	}

	static void disable_half_transfer_interrupt()
	{
		clear_periph_reg_bit<CHAN_CCR, DMA_CCR_HTIE>();
	}

	static void enable_transfer_complete_interrupt()
	{
		set_periph_reg_bit<CHAN_CCR, DMA_CCR_TCIE>();
	}

	static void disable_transfer_complete_interrupt()
	{
		clear_periph_reg_bit<CHAN_CCR, DMA_CCR_TCIE>();
	}

	static void enable()
	{
		set_periph_reg_bit<CHAN_CCR, DMA_CCR_EN>();
	}

	static void disable()
	{
		clear_periph_reg_bit<CHAN_CCR, DMA_CCR_EN>();
	}

	static void set_number_of_data(uint32_t value)
	{
		HL_UI32REG(CHAN_CNDTR) = value;
	}

	static void set_peripheral_address(uintptr_t address)
	{
		HL_UI32REG(CHAN_CPAR) = address;
	}

	static void set_memory_address(volatile void *address)
	{
		HL_UI32REG(CHAN_CMAR) = (uint32_t)address;
	}

private:
	static constexpr uintptr_t chan = ChanHelperEx::chan;
	static constexpr uintptr_t dma = detailed::DMAHelper<DmaN>::dma;
	static constexpr uintptr_t CHAN_CCR = chan + offsetof(DMA_Channel_TypeDef, CCR);
	static constexpr uintptr_t CHAN_CNDTR = chan + offsetof(DMA_Channel_TypeDef, CNDTR);
	static constexpr uintptr_t CHAN_CPAR = chan + offsetof(DMA_Channel_TypeDef, CPAR);
	static constexpr uintptr_t CHAN_CMAR = chan + offsetof(DMA_Channel_TypeDef, CMAR);
	static constexpr uintptr_t DMA_ISR = dma + offsetof(DMA_TypeDef, ISR);
	static constexpr uintptr_t DMA_IFCR = dma + offsetof(DMA_TypeDef, IFCR);
};

namespace detailed {

#ifdef RCC_AHBENR_DMA1EN
template <> struct DMAHelper<1>
{
	static constexpr uint32_t RccBit = RCC_AHBENR_DMA1EN;
	static constexpr uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBENR);
#ifdef RCC_AHBRSTR_DMA1RST
	static constexpr uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBRSTR);
#endif

	static constexpr uintptr_t dma = DMA1_BASE;
};

template <> struct DMAChanHelperEx<1, 1>
{
	// DMA_Channel_TypeDef
	static constexpr uintptr_t chan = DMA1_Channel1_BASE;
	static constexpr IRQn_Type IRQn = DMA1_Channel1_IRQn;
};

template <> struct DMAChanHelperEx<1, 2>
{
	static constexpr uintptr_t chan = DMA1_Channel2_BASE;
	static constexpr IRQn_Type IRQn = DMA1_Channel2_IRQn;
};

template <> struct DMAChanHelperEx<1, 3>
{
	static constexpr uintptr_t chan = DMA1_Channel3_BASE;
	static constexpr IRQn_Type IRQn = DMA1_Channel3_IRQn;
};

template <> struct DMAChanHelperEx<1, 4>
{
	static constexpr uintptr_t chan = DMA1_Channel4_BASE;
	static constexpr IRQn_Type IRQn = DMA1_Channel4_IRQn;
};

template <> struct DMAChanHelperEx<1, 5>
{
	static constexpr uintptr_t chan = DMA1_Channel5_BASE;
	static constexpr IRQn_Type IRQn = DMA1_Channel5_IRQn;
};

template <> struct DMAChanHelperEx<1, 6>
{
	static constexpr uintptr_t chan = DMA1_Channel6_BASE;
	static constexpr IRQn_Type IRQn = DMA1_Channel6_IRQn;
};

template <> struct DMAChanHelperEx<1, 7>
{
	static constexpr uintptr_t chan = DMA1_Channel7_BASE;
	static constexpr IRQn_Type IRQn = DMA1_Channel7_IRQn;
};

#endif

#ifdef RCC_AHBENR_DMA2EN

template <> struct DMAHelper<2>
{
	static constexpr uint32_t RccBit = RCC_AHBENR_DMA2EN;
	static constexpr uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBENR);
	static constexpr uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBRSTR);

	static constexpr uintptr_t dma = DMA2_BASE;
};

template <> struct DMAChanHelperEx<2, 1>
{
	static constexpr uintptr_t chan = DMA2_Channel1_BASE;
	static constexpr IRQn_Type IRQn = DMA2_Channel1_IRQn;
};

template <> struct DMAChanHelperEx<2, 2>
{
	static constexpr uintptr_t chan = DMA2_Channel2_BASE;
	static constexpr IRQn_Type IRQn = DMA2_Channel2_IRQn;
};

template <> struct DMAChanHelperEx<2, 3>
{
	static constexpr uintptr_t chan = DMA2_Channel3_BASE;
	static constexpr IRQn_Type IRQn = DMA2_Channel3_IRQn;
};

template <> struct DMAChanHelperEx<2, 4>
{
	static constexpr uintptr_t chan = DMA2_Channel4_BASE;
	static constexpr IRQn_Type IRQn = DMA2_Channel4_IRQn;
};

template <> struct DMAChanHelperEx<2, 5>
{
	static constexpr uintptr_t chan = DMA2_Channel5_BASE;
	static constexpr IRQn_Type IRQn = DMA2_Channel5_IRQn;
};

#endif

template <> struct DMAChanHelper<1>
{
	static constexpr uint32_t TransferErrorFlag    = DMA_ISR_TEIF1;
	static constexpr uint32_t HalfTransferFlag     = DMA_ISR_HTIF1;
	static constexpr uint32_t TransferCompleteFlag = DMA_ISR_TCIF1;
	static constexpr uint32_t GlobalInterruptFlag  = DMA_ISR_GIF1;
};

template <> struct DMAChanHelper<2>
{
	static constexpr uint32_t TransferErrorFlag    = DMA_ISR_TEIF2;
	static constexpr uint32_t HalfTransferFlag     = DMA_ISR_HTIF2;
	static constexpr uint32_t TransferCompleteFlag = DMA_ISR_TCIF2;
	static constexpr uint32_t GlobalInterruptFlag  = DMA_ISR_GIF2;
};

template <> struct DMAChanHelper<3>
{
	static constexpr uint32_t TransferErrorFlag    = DMA_ISR_TEIF3;
	static constexpr uint32_t HalfTransferFlag     = DMA_ISR_HTIF3;
	static constexpr uint32_t TransferCompleteFlag = DMA_ISR_TCIF3;
	static constexpr uint32_t GlobalInterruptFlag  = DMA_ISR_GIF3;
};

template <> struct DMAChanHelper<4>
{
	static constexpr uint32_t TransferErrorFlag    = DMA_ISR_TEIF4;
	static constexpr uint32_t HalfTransferFlag     = DMA_ISR_HTIF4;
	static constexpr uint32_t TransferCompleteFlag = DMA_ISR_TCIF4;
	static constexpr uint32_t GlobalInterruptFlag  = DMA_ISR_GIF4;
};

template <> struct DMAChanHelper<5>
{
	static constexpr uint32_t TransferErrorFlag    = DMA_ISR_TEIF5;
	static constexpr uint32_t HalfTransferFlag     = DMA_ISR_HTIF5;
	static constexpr uint32_t TransferCompleteFlag = DMA_ISR_TCIF5;
	static constexpr uint32_t GlobalInterruptFlag  = DMA_ISR_GIF5;
};

template <> struct DMAChanHelper<6>
{
	static constexpr uint32_t TransferErrorFlag    = DMA_ISR_TEIF6;
	static constexpr uint32_t HalfTransferFlag     = DMA_ISR_HTIF6;
	static constexpr uint32_t TransferCompleteFlag = DMA_ISR_TCIF6;
	static constexpr uint32_t GlobalInterruptFlag  = DMA_ISR_GIF6;
};

template <> struct DMAChanHelper<7>
{
	static constexpr uint32_t TransferErrorFlag    = DMA_ISR_TEIF7;
	static constexpr uint32_t HalfTransferFlag     = DMA_ISR_HTIF7;
	static constexpr uint32_t TransferCompleteFlag = DMA_ISR_TCIF7;
	static constexpr uint32_t GlobalInterruptFlag  = DMA_ISR_GIF7;
};


} // namespace detailed


#ifdef RCC_AHBENR_DMA1EN
typedef DMA_<1> Dma1;

typedef DMAChannel<1, 1> DMA1_Chan1;
typedef DMAChannel<1, 2> DMA1_Chan2;
typedef DMAChannel<1, 3> DMA1_Chan3;
typedef DMAChannel<1, 4> DMA1_Chan4;
typedef DMAChannel<1, 5> DMA1_Chan5;
typedef DMAChannel<1, 6> DMA1_Chan6;
typedef DMAChannel<1, 7> DMA1_Chan7;

#endif

#ifdef RCC_AHBENR_DMA2EN

typedef DMA_<2> Dma2;

typedef DMAChannel<2, 1> DMA2_Chan1;
typedef DMAChannel<2, 2> DMA2_Chan2;
typedef DMAChannel<2, 3> DMA2_Chan3;
typedef DMAChannel<2, 4> DMA2_Chan4;
typedef DMAChannel<2, 5> DMA2_Chan5;

#endif

} // namespace hl
