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
	Low      = 0b00,
	Medium   = 0b01,
	High     = 0b10,
	VeryHigh = 0b11,
};

enum class DMADataSize
{
	_8bit  = 0,
	_16bit = 1,
	_32bit = 2
};

enum class DMADirection
{
	PeriphToMem = 0,
	MemToPeriph = 1,
	MemToMem    = 2
};

template <unsigned N>
class DMAController : public Device<detailed::DMAHelper<N>>
{
};

#ifdef HL_STM32F4XX

template <unsigned DmaN, unsigned ChanN>
class DMAStream
{
private:
	typedef detailed::DMAChanHelper<ChanN> ChanHelper;
	typedef detailed::DMAChanHelperEx<DmaN, ChanN> ChanHelperEx;

public:
	static constexpr IRQn_Type IRQn = ChanHelperEx::IRQn;

	static void enable()
	{
		set_periph_reg_bit<cr, DMA_SxCR_EN>();
	}

	static void disable()
	{
		clear_periph_reg_bit<cr, DMA_SxCR_EN>();
	}

	static void set_channel(uint8_t channel)
	{
		set_value_by_mask<uint32_t>(cr, DMA_SxCR_CHSEL, ((uint32_t)channel) << DMA_SxCR_CHSEL_Pos);
	}

	static void set_memory_address(volatile void *address)
	{
		HL_UI32REG(m0ar) = (uint32_t)address;
	}

	static void set_memory1_address(volatile void *address)
	{
		HL_UI32REG(m1ar) = (uint32_t)address;
	}

	static void set_peripheral_address(uintptr_t address)
	{
		HL_UI32REG(par) = address;
	}

	static void set_number_of_data(uint32_t value)
	{
		HL_UI32REG(ndtr) = value;
	}

	static uint32_t get_number_of_data()
	{
		return HL_UI32REG(ndtr);
	}

	static void set_direction(DMADirection direction)
	{
		set_value_by_mask<uint32_t>(cr, DMA_SxCR_DIR, ((uint32_t)direction) << DMA_SxCR_DIR_Pos);
	}

	static void set_peripheral_size(DMADataSize size)
	{
		set_value_by_mask<uint32_t>(cr, DMA_SxCR_PSIZE, ((uint32_t)size) << DMA_SxCR_PSIZE_Pos);
	}

	static void enable_peripheral_increment()
	{
		set_periph_reg_bit<cr, DMA_SxCR_PINC>();
	}

	static void disable_peripheral_increment()
	{
		clear_periph_reg_bit<cr, DMA_SxCR_PINC>();
	}

	static void enable_memory_increment()
	{
		set_periph_reg_bit<cr, DMA_SxCR_MINC>();
	}

	static void disable_memory_increment()
	{
		clear_periph_reg_bit<cr, DMA_SxCR_MINC>();
	}

	static void set_memory_size(DMADataSize size)
	{
		set_value_by_mask<uint32_t>(cr, DMA_SxCR_MSIZE, ((uint32_t)size) << DMA_SxCR_MSIZE_Pos);
	}

	static void enable_transfer_error_interrupt()
	{
		set_periph_reg_bit<cr, DMA_SxCR_TEIE>();
	}

	static void disable_transfer_error_interrupt()
	{
		clear_periph_reg_bit<cr, DMA_SxCR_TEIE>();
	}

	static void enable_transfer_complete_interrupt()
	{
		set_periph_reg_bit<cr, DMA_SxCR_TCIE>();
	}

	static void disable_transfer_complete_interrupt()
	{
		clear_periph_reg_bit<cr, DMA_SxCR_TCIE>();
	}

	static bool get_transfer_error_flag()
	{
		return get_periph_reg_bit<isr, ChanHelper::TransferErrorFlag>();
	}

	static void clear_transfer_error_flag()
	{
		HL_UI32REG(ifcr) = ChanHelper::TransferErrorFlag;
	}

	static bool get_transfer_complete_flag()
	{
		return get_periph_reg_bit<isr, ChanHelper::TransferCompleteFlag>();
	}

	static void clear_transfer_complete_flag()
	{
		HL_UI32REG(ifcr) = ChanHelper::TransferCompleteFlag;
	}



private:
	static constexpr uintptr_t stream_base = ChanHelperEx::chan;
	static constexpr uintptr_t dma_base    = detailed::DMAHelper<DmaN>::dma;

	static constexpr uintptr_t isr         = dma_base + ChanHelper::isr_offset;
	static constexpr uintptr_t ifcr        = dma_base + ChanHelper::ifcr_offset;

	static constexpr uintptr_t cr          = stream_base + offsetof(DMA_Stream_TypeDef, CR);
	static constexpr uintptr_t par         = stream_base + offsetof(DMA_Stream_TypeDef, PAR);
	static constexpr uintptr_t m0ar        = stream_base + offsetof(DMA_Stream_TypeDef, M0AR);
	static constexpr uintptr_t m1ar        = stream_base + offsetof(DMA_Stream_TypeDef, M1AR);
	static constexpr uintptr_t ndtr        = stream_base + offsetof(DMA_Stream_TypeDef, NDTR);
};

namespace detailed {

template <> struct DMAHelper<1>
{
	static constexpr uint32_t RccBit = RCC_AHB1ENR_DMA1EN;
	static constexpr uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, AHB1ENR);
	static constexpr uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, AHB1RSTR);
	static constexpr uintptr_t dma = DMA1_BASE;
};

template <> struct DMAHelper<2>
{
	static constexpr uint32_t RccBit = RCC_AHB1ENR_DMA2EN;
	static constexpr uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, AHB1ENR);
	static constexpr uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, AHB1RSTR);
	static constexpr uintptr_t dma = DMA2_BASE;
};

template <> struct DMAChanHelperEx<1, 0>
{
	static constexpr uintptr_t chan = DMA1_Stream0_BASE;
	static constexpr IRQn_Type IRQn = DMA1_Stream0_IRQn;
};

template <> struct DMAChanHelperEx<1, 1>
{
	static constexpr uintptr_t chan = DMA1_Stream1_BASE;
	static constexpr IRQn_Type IRQn = DMA1_Stream1_IRQn;
};

template <> struct DMAChanHelperEx<1, 2>
{
	static constexpr uintptr_t chan = DMA1_Stream2_BASE;
	static constexpr IRQn_Type IRQn = DMA1_Stream2_IRQn;
};

template <> struct DMAChanHelperEx<1, 3>
{
	static constexpr uintptr_t chan = DMA1_Stream3_BASE;
	static constexpr IRQn_Type IRQn = DMA1_Stream3_IRQn;
};

template <> struct DMAChanHelperEx<1, 4>
{
	static constexpr uintptr_t chan = DMA1_Stream4_BASE;
	static constexpr IRQn_Type IRQn = DMA1_Stream4_IRQn;
};

template <> struct DMAChanHelperEx<1, 5>
{
	static constexpr uintptr_t chan = DMA1_Stream5_BASE;
	static constexpr IRQn_Type IRQn = DMA1_Stream5_IRQn;
};

template <> struct DMAChanHelperEx<1, 6>
{
	static constexpr uintptr_t chan = DMA1_Stream6_BASE;
	static constexpr IRQn_Type IRQn = DMA1_Stream6_IRQn;
};

template <> struct DMAChanHelperEx<1, 7>
{
	static constexpr uintptr_t chan = DMA1_Stream7_BASE;
	static constexpr IRQn_Type IRQn = DMA1_Stream7_IRQn;
};

template <> struct DMAChanHelperEx<2, 0>
{
	static constexpr uintptr_t chan = DMA2_Stream0_BASE;
	static constexpr IRQn_Type IRQn = DMA2_Stream0_IRQn;
};

template <> struct DMAChanHelperEx<2, 1>
{
	static constexpr uintptr_t chan = DMA2_Stream1_BASE;
	static constexpr IRQn_Type IRQn = DMA2_Stream1_IRQn;
};

template <> struct DMAChanHelperEx<2, 2>
{
	static constexpr uintptr_t chan = DMA2_Stream2_BASE;
	static constexpr IRQn_Type IRQn = DMA2_Stream2_IRQn;
};

template <> struct DMAChanHelperEx<2, 3>
{
	static constexpr uintptr_t chan = DMA2_Stream3_BASE;
	static constexpr IRQn_Type IRQn = DMA2_Stream3_IRQn;
};

template <> struct DMAChanHelperEx<2, 4>
{
	static constexpr uintptr_t chan = DMA2_Stream4_BASE;
	static constexpr IRQn_Type IRQn = DMA2_Stream4_IRQn;
};

template <> struct DMAChanHelperEx<2, 5>
{
	static constexpr uintptr_t chan = DMA2_Stream5_BASE;
	static constexpr IRQn_Type IRQn = DMA2_Stream5_IRQn;
};

template <> struct DMAChanHelperEx<2, 6>
{
	static constexpr uintptr_t chan = DMA2_Stream6_BASE;
	static constexpr IRQn_Type IRQn = DMA2_Stream6_IRQn;
};

template <> struct DMAChanHelperEx<2, 7>
{
	static constexpr uintptr_t chan = DMA2_Stream7_BASE;
	static constexpr IRQn_Type IRQn = DMA2_Stream7_IRQn;
};


template <> struct DMAChanHelper<0>
{
	static constexpr uintptr_t isr_offset           = offsetof(DMA_TypeDef, LISR);
	static constexpr uintptr_t ifcr_offset          = offsetof(DMA_TypeDef, LIFCR);
	static constexpr uint32_t  TransferErrorFlag    = DMA_LISR_TEIF0;
	static constexpr uint32_t  HalfTransferFlag     = DMA_LISR_HTIF0;
	static constexpr uint32_t  TransferCompleteFlag = DMA_LISR_TCIF0;
	static constexpr uint32_t  DirectModeErrorFlag  = DMA_LISR_DMEIF0;
};


template <> struct DMAChanHelper<1>
{
	static constexpr uintptr_t isr_offset           = offsetof(DMA_TypeDef, LISR);
	static constexpr uintptr_t ifcr_offset          = offsetof(DMA_TypeDef, LIFCR);
	static constexpr uint32_t  TransferErrorFlag    = DMA_LISR_TEIF1;
	static constexpr uint32_t  HalfTransferFlag     = DMA_LISR_HTIF1;
	static constexpr uint32_t  TransferCompleteFlag = DMA_LISR_TCIF1;
	static constexpr uint32_t  DirectModeErrorFlag  = DMA_LISR_DMEIF1;
};

template <> struct DMAChanHelper<2>
{
	static constexpr uintptr_t isr_offset           = offsetof(DMA_TypeDef, LISR);
	static constexpr uintptr_t ifcr_offset          = offsetof(DMA_TypeDef, LIFCR);
	static constexpr uint32_t  TransferErrorFlag    = DMA_LISR_TEIF2;
	static constexpr uint32_t  HalfTransferFlag     = DMA_LISR_HTIF2;
	static constexpr uint32_t  TransferCompleteFlag = DMA_LISR_TCIF2;
	static constexpr uint32_t  DirectModeErrorFlag  = DMA_LISR_DMEIF2;
};

template <> struct DMAChanHelper<3>
{
	static constexpr uintptr_t isr_offset           = offsetof(DMA_TypeDef, LISR);
	static constexpr uintptr_t ifcr_offset          = offsetof(DMA_TypeDef, LIFCR);
	static constexpr uint32_t  TransferErrorFlag    = DMA_LISR_TEIF3;
	static constexpr uint32_t  HalfTransferFlag     = DMA_LISR_HTIF3;
	static constexpr uint32_t  TransferCompleteFlag = DMA_LISR_TCIF3;
	static constexpr uint32_t  DirectModeErrorFlag  = DMA_LISR_DMEIF3;
};

template <> struct DMAChanHelper<4>
{
	static constexpr uintptr_t isr_offset           = offsetof(DMA_TypeDef, HISR);
	static constexpr uintptr_t ifcr_offset          = offsetof(DMA_TypeDef, HIFCR);
	static constexpr uint32_t  TransferErrorFlag    = DMA_HISR_TEIF4;
	static constexpr uint32_t  HalfTransferFlag     = DMA_HISR_HTIF4;
	static constexpr uint32_t  TransferCompleteFlag = DMA_HISR_TCIF4;
	static constexpr uint32_t  DirectModeErrorFlag  = DMA_HISR_DMEIF4;
};

template <> struct DMAChanHelper<5>
{
	static constexpr uintptr_t isr_offset           = offsetof(DMA_TypeDef, HISR);
	static constexpr uintptr_t ifcr_offset          = offsetof(DMA_TypeDef, HIFCR);
	static constexpr uint32_t  TransferErrorFlag    = DMA_HISR_TEIF5;
	static constexpr uint32_t  HalfTransferFlag     = DMA_HISR_HTIF5;
	static constexpr uint32_t  TransferCompleteFlag = DMA_HISR_TCIF5;
	static constexpr uint32_t  DirectModeErrorFlag  = DMA_HISR_DMEIF5;
};

template <> struct DMAChanHelper<6>
{
	static constexpr uintptr_t isr_offset           = offsetof(DMA_TypeDef, HISR);
	static constexpr uintptr_t ifcr_offset          = offsetof(DMA_TypeDef, HIFCR);
	static constexpr uint32_t  TransferErrorFlag    = DMA_HISR_TEIF6;
	static constexpr uint32_t  HalfTransferFlag     = DMA_HISR_HTIF6;
	static constexpr uint32_t  TransferCompleteFlag = DMA_HISR_TCIF6;
	static constexpr uint32_t  DirectModeErrorFlag  = DMA_HISR_DMEIF6;
};

template <> struct DMAChanHelper<7>
{
	static constexpr uintptr_t isr_offset           = offsetof(DMA_TypeDef, HISR);
	static constexpr uintptr_t ifcr_offset          = offsetof(DMA_TypeDef, HIFCR);
	static constexpr uint32_t  TransferErrorFlag    = DMA_HISR_TEIF7;
	static constexpr uint32_t  HalfTransferFlag     = DMA_HISR_HTIF7;
	static constexpr uint32_t  TransferCompleteFlag = DMA_HISR_TCIF7;
	static constexpr uint32_t  DirectModeErrorFlag  = DMA_HISR_DMEIF7;
};


} // namespace detailed


typedef DMAController<1> Dma1_Controller;

typedef DMAStream<1, 0> Dma1_Stream0;
typedef DMAStream<1, 1> Dma1_Stream1;
typedef DMAStream<1, 2> Dma1_Stream2;
typedef DMAStream<1, 3> Dma1_Stream3;
typedef DMAStream<1, 4> Dma1_Stream4;
typedef DMAStream<1, 5> Dma1_Stream5;
typedef DMAStream<1, 6> Dma1_Stream6;
typedef DMAStream<1, 7> Dma1_Stream7;


typedef DMAController<2> Dma2_Controller;

typedef DMAStream<2, 0> Dma2_Stream0;
typedef DMAStream<2, 1> Dma2_Stream1;
typedef DMAStream<2, 2> Dma2_Stream2;
typedef DMAStream<2, 3> Dma2_Stream3;
typedef DMAStream<2, 4> Dma2_Stream4;
typedef DMAStream<2, 5> Dma2_Stream5;
typedef DMAStream<2, 6> Dma2_Stream6;
typedef DMAStream<2, 7> Dma2_Stream7;

#else // ifdef HL_STM32F4XX

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
		set_value_by_mask<uint32_t>(CHAN_CCR, DMA_CCR_PL, ((uint32_t)priority << DMA_CCR_PL_Pos));
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
		set_value_by_mask<uint32_t>(CHAN_CCR, DMA_CCR_DIR, ((uint32_t)direction << DMA_CCR_DIR_Pos));
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

	static void set_memory_address(uintptr_t address)
	{
		HL_UI32REG(CHAN_CMAR) = address;
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

#ifdef DMA1
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

#ifdef DMA2

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


#ifdef DMA1
typedef DMAController<1> Dma1_Controller;

typedef DMAChannel<1, 1> Dma1_Chan1;
typedef DMAChannel<1, 2> Dma1_Chan2;
typedef DMAChannel<1, 3> Dma1_Chan3;
typedef DMAChannel<1, 4> Dma1_Chan4;
typedef DMAChannel<1, 5> Dma1_Chan5;
typedef DMAChannel<1, 6> Dma1_Chan6;
typedef DMAChannel<1, 7> Dma1_Chan7;

#endif

#ifdef DMA2

typedef DMAController<2> Dma2_Controller;

typedef DMAChannel<2, 1> Dma2_Chan1;
typedef DMAChannel<2, 2> Dma2_Chan2;
typedef DMAChannel<2, 3> Dma2_Chan3;
typedef DMAChannel<2, 4> Dma2_Chan4;
typedef DMAChannel<2, 5> Dma2_Chan5;

#endif

#endif

} // namespace hl
