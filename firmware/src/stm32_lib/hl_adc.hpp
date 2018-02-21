#include "hl_device.hpp"

namespace hl {

// fwd.
namespace detailed {
	template <unsigned N> struct AdcDevice;
	template <unsigned N> struct AdcRegisters;
	template <unsigned N> struct AdcCommonHelper;
}

enum class ADCResolution
{
	_12bit = 0b00,
	_10bit = 0b01,
	_8bit  = 0b10,
	_6bit  = 0b11,
};

enum class ADCDataAlignment
{
	Right = 0b0,
	Left  = 0b1,
};

enum class ADCExtTrigger
{
	Disable         = 0b00,
	RisingEdge      = 0b01,
	FallingEdge     = 0b10,
	RisingOrFalling = 0b11,
};

#if defined(HL_STM32L1XX)
enum class ADCExtEventRegular
{
	TIM9_CC2  = 0b0000 << 24,
	TIM9_TRGO = 0b0001 << 24,
	TIM2_CC3  = 0b0010 << 24,
	TIM2_CC2  = 0b0011 << 24,
	TIM3_TRGO = 0b0100 << 24,
	TIM4_CC4  = 0b0101 << 24,
	TIM2_TRGO = 0b0110 << 24,
	TIM3_CC1  = 0b0111 << 24,
	TIM3_CC3  = 0b1000 << 24,
	TIM4_TRGO = 0b1001 << 24,
	TIM6_TRGO = 0b1010 << 24,
	Reserved1 = 0b1011 << 24,
	Reserved2 = 0b1100 << 24,
	Reserved3 = 0b1101 << 24,
	Reserved4 = 0b1110 << 24,
	EXTI_line = 0b1111 << 24,
};

#elif defined(HL_STM32F3XX)
enum class ADC12ExtEventRegular
{
	TIM1_CC1              = 0b0000 << 6,
	TIM1_CC2              = 0b0001 << 6,
	TIM1_CC3orTIM20_TRGO  = 0b0010 << 6,
	TIM2_CC2orTIM20_TRGO2 = 0b0011 << 6,
	TIM3_TRGO             = 0b0100 << 6,
	TIM4_CC4orTIM20_CC1   = 0b0101 << 6,
	EXTI_11               = 0b0110 << 6,
	TIM8_TRGO             = 0b0111 << 6,
	TIM8_TRGO2            = 0b1000 << 6,
	TIM1_TRGO             = 0b1001 << 6,
	TIM1_TRGO2            = 0b1010 << 6,
	TIM2_TRGO             = 0b1011 << 6,
	TIM4_TRGO             = 0b1100 << 6,
	TIM6_TRGOorTIM20_CC2  = 0b1101 << 6,
	TIM15_TRGO            = 0b1110 << 6,
	TIM3_CC4orTIM20_CC3   = 0b1111 << 6,
};
#endif




#if defined(HL_STM32L1XX)

enum class AdcPrescaler
{
	_1 = 0b00 << 16,
	_2 = 0b01 << 16,
	_4 = 0b10 << 16,
};

inline void adc_set_prescaler(AdcPrescaler prescaler)
{
	set_value_by_mask(ADC->CCR, ADC_CCR_ADCPRE, (uint32_t)prescaler);
}

enum class ADCDelay
{
	NoDelay     = 0b000 << 4,
	UntilRead   = 0b001 << 4,
	_7_cycles   = 0b010 << 4,
	_15_cycles  = 0b011 << 4,
	_31_cycles  = 0b100 << 4,
	_63_cycles  = 0b101 << 4,
	_127_cycles = 0b110 << 4,
	_255_cycles = 0b111 << 4,
};

enum class DMABank
{
	A = 0b0 << 2,
	B = 0b1 << 2,
};

enum class ADCSampleRate
{
	_4_cycles   = 0x0,
	_9_cycles   = 0x1,
	_16_cycles  = 0x2,
	_24_cycles  = 0x3,
	_48_cycles  = 0x4,
	_96_cycles  = 0x5,
	_192_cycles = 0x6,
	_384_cycles = 0x7,
};

enum class WatchDogMode
{
	None,
	AllInjectedChannels            = ADC_CR1_JAWDEN,
	AllRegularChannels             = ADC_CR1_AWDEN,
	AllRegularAndInjectedChannels  = ADC_CR1_JAWDEN|ADC_CR1_AWDEN,
	SingleInjectedChannel          = ADC_CR1_AWDSGL|ADC_CR1_JAWDEN,
	SingleRegularChannel           = ADC_CR1_AWDSGL|ADC_CR1_AWDEN,
	SingleRegularOrInjectedChannel = ADC_CR1_AWDSGL|ADC_CR1_JAWDEN|ADC_CR1_AWDEN,
};

#elif defined(HL_STM32F3XX)

enum class AdcCalibr
{
	SingleEnded,
	Differential
};

enum class ADCSampleRate
{
	_1_5_cycles   = 0b000,
	_2_5_cycles   = 0b001,
	_4_5_cycles   = 0b010,
	_7_5_cycles   = 0b011,
	_19_5_cycles  = 0b100,
	_61_5_cycles  = 0b101,
	_181_5_cycles = 0b110,
	_601_5_cycles = 0b111,
};

#endif

template <unsigned N>
class Adc : public Device<detailed::AdcDevice<N>>
{
public:

#if defined(HL_STM32F3XX)
	template <class Delay>
	static void power_on()
	{
		set_value_by_mask<uint32_t>(ADC_CR, ADC_CR_ADVREGEN, 0);
		set_value_by_mask<uint32_t>(ADC_CR, ADC_CR_ADVREGEN, ADC_CR_ADVREGEN_0);
		Delay::exec_us(20); // 10 us is ok, but 20 is better :-)
	}

	static void power_off()
	{
		set_value_by_mask<uint32_t>(ADC_CR, ADC_CR_ADVREGEN, 0);
		set_value_by_mask<uint32_t>(ADC_CR, ADC_CR_ADVREGEN, ADC_CR_ADVREGEN_1);
	}
#endif // HL_STM32F3XX

	static void enable()
	{
#if defined(HL_STM32L1XX)
		set_periph_reg_bit<ADC_CR2, ADC_CR2_ADON>();
		while (!get_periph_reg_bit<ADC_SR, ADC_SR_ADONS>()) {}
#elif defined(HL_STM32F3XX)
		set_periph_reg_bit<ADC_ISR, ADC_ISR_ADRD>(); // clear ADRD
		set_periph_reg_bit<ADC_CR, ADC_CR_ADEN>();
		while (!get_periph_reg_bit<ISR, ADC_ISR_ADRD>()) {}
#endif
	}

	static void disable()
	{
#if defined(HL_STM32L1XX)
		clear_periph_reg_bit<ADC_CR2, ADC_CR2_ADON>();
		while (get_periph_reg_bit<ADC_SR, ADC_SR_ADONS>()) {}
#elif defined(HL_STM32F3XX)
		if (!get_periph_reg_bit<ADC_CR, ADC_CR_ADEN>()) return;
		set_periph_reg_bit<ADC_CR, ADC_CR_ADDIS>();
		while (get_periph_reg_bit<ADC_CR, ADC_CR_ADEN>()) {}
#endif
	}

	static bool is_enabled()
	{
#if defined(HL_STM32L1XX)
		return get_periph_reg_bit<ADC_CR2, ADC_CR2_ADON>();
#elif defined(HL_STM32F3XX)
		return get_periph_reg_bit<ADC_ISR, ADC_ISR_ADRD>();
#endif
	}

	// status

	static bool get_ready_flag()
	{
#if defined(HL_STM32L1XX)
		return get_periph_reg_bit<ADC_SR, ADC_SR_ADONS>();
#elif defined(HL_STM32F3XX)
		return get_periph_reg_bit<ADC_ISR, ADC_ISR_ADRD>();
#endif
	}

#if defined(HL_STM32F3XX)
	static void calibrate(AdcCalibr type)
	{
		disable();
		switch (type)
		{
		case AdcCalibr::SingleEnded:
			clear_periph_reg_bit<ADC_CR, ADC_CR_ADCALDIF>();
			break;

		case AdcCalibr::Differential:
			set_periph_reg_bit<ADC_CR, ADC_CR_ADCALDIF>();
			break;
		}

		set_periph_reg_bit<ADC_CR, ADC_CR_ADCAL>();

		while (get_periph_reg_bit<ADC_CR, ADC_CR_ADCAL>()) {}
		for (unsigned i = 0; i < 100; i++) __NOP();
	}
#endif


	static bool get_overrun_flag()
	{
#if defined(HL_STM32L1XX)
		return get_periph_reg_bit<ADC_SR, ADC_SR_OVR>();
#elif defined(HL_STM32F3XX)
		return get_periph_reg_bit<ADC_ISR, ADC_ISR_OVR>();
#endif
	}

	static void clear_overrun_flag()
	{
#if defined(HL_STM32L1XX)
		HL_UI32REG(ADC_SR) = ~ADC_SR_OVR;
#elif defined(HL_STM32F3XX)
		HL_UI32REG(ADC_ISR) = ~ADC_ISR_OVR;
#endif
	}

	static bool get_eoc_flag()
	{
#if defined(HL_STM32L1XX)
		return get_periph_reg_bit<ADC_SR, ADC_SR_EOC>();
#elif defined(HL_STM32F3XX)
		return get_periph_reg_bit<ADC_ISR, ADC_ISR_EOC>();
#endif
	}

	static void clear_eoc_flag()
	{
#if defined(HL_STM32L1XX)
		HL_UI32REG(ADC_SR) = ~ADC_SR_EOC;
#elif defined(HL_STM32F3XX)
		HL_UI32REG(ADC_ISR) = ~ADC_ISR_EOC;
#endif
	}

#if defined(HL_STM32L1XX)
	static bool get_analog_watchdog_flag()
	{
		return get_periph_reg_bit<ADC_SR, ADC_SR_AWD>();
	}

	static void clear_analog_watchdog_flag()
	{
		HL_UI32REG(ADC_SR) = ~ADC_SR_AWD;
	}

#elif defined(HL_STM32F3XX)
	static bool get_analog_watchdog1_flag()
	{
		return get_periph_reg_bit<ADC_ISR, ADC_ISR_AWD1>();
	}

	static void clear_analog_watchdog1_flag()
	{
		HL_UI32REG(ADC_ISR) = ~ADC_ISR_AWD1;
	}

	static bool get_analog_watchdog2_flag()
	{
		return get_periph_reg_bit<ADC_ISR, ADC_ISR_AWD2>();
	}

	static void clear_analog_watchdog2_flag()
	{
		HL_UI32REG(ADC_ISR) = ~ADC_ISR_AWD2;
	}

	static bool get_analog_watchdog3_flag()
	{
		return get_periph_reg_bit<ADC_ISR, ADC_ISR_AWD3>();
	}

	static void clear_analog_watchdog3_flag()
	{
		HL_UI32REG(ADC_ISR) = ~ADC_ISR_AWD3;
	}

#endif

	// configuration

	static void enable_overrun_interrupt()
	{
#if defined(HL_STM32L1XX)
		set_periph_reg_bit<ADC_CR1, ADC_CR1_OVRIE>();
#elif defined(HL_STM32F3XX)
		set_periph_reg_bit<ADC_IER, ADC_IER_OVR>();
#endif
	}

	static void disable_overrun_interrupt()
	{
#if defined(HL_STM32L1XX)
		clear_periph_reg_bit<ADC_CR1, ADC_CR1_OVRIE>();
#elif defined(HL_STM32F3XX)
		clear_periph_reg_bit<ADC_IER, ADC_IER_OVR>();
#endif
	}

	static void set_resolution(const ADCResolution resolution)
	{
#if defined(HL_STM32L1XX)
		set_value_by_mask<uint32_t>(ADC_CR1, ADC_CR1_RES, (uint32_t)resolution << 24);
#elif defined(HL_STM32F3XX)
		set_value_by_mask<uint32_t>(ADC_CFGR, ADC_CFGR_RES, (uint32_t)resolution << 3);
#endif
	}

	static void set_data_alignment(const ADCDataAlignment alignment)
	{
#if defined(HL_STM32L1XX)
		set_value_by_mask<uint32_t>(ADC_CR2, ADC_CR2_ALIGN, (uint32_t)alignment << 11);
#elif defined(HL_STM32F3XX)
		set_value_by_mask<uint32_t>(ADC_CFGR, ADC_CFGR_ALIGN, (uint32_t)alignment << 5);
#endif
	}

	static void enable_continuous_conversion_mode()
	{
#if defined(HL_STM32L1XX)
		set_periph_reg_bit<ADC_CR2, ADC_CR2_CONT>();
#elif defined(HL_STM32F3XX)
		set_periph_reg_bit<ADC_CFGR, ADC_CFGR_CONT>();
#endif
	}

	static void disable_continuous_conversion_mode()
	{
#if defined(HL_STM32L1XX)
		clear_periph_reg_bit<ADC_CR2, ADC_CR2_CONT>();
#elif defined(HL_STM32F3XX)
		clear_periph_reg_bit<ADC_CFGR, ADC_CFGR_CONT>();
#endif
	}

#if defined(HL_STM32L1XX)
	static void enable_scan_mode()
	{
		set_periph_reg_bit<ADC_CR1, ADC_CR1_SCAN>();
	}

	static void disable_scan_mode()
	{
		clear_periph_reg_bit<ADC_CR1, ADC_CR1_SCAN>();
	}
#endif

	static void enable_dma_circular_mode()
	{
#if defined(HL_STM32L1XX)
		set_periph_reg_bit<ADC_CR2, ADC_CR2_DDS>();
#elif defined(HL_STM32F3XX)
		set_periph_reg_bit<ADC_CFGR, ADC_CFGR_DMACFG>();
#endif
	}

	static void disable_dma_disable()
	{
#if defined(HL_STM32L1XX)
		clear_periph_reg_bit<ADC_CR2, ADC_CR2_DDS>();
#elif defined(HL_STM32F3XX)
		clear_periph_reg_bit<ADC_CFGR, ADC_CFGR_DMACFG>();
#endif
	}

	static void enable_dma()
	{
#if defined(HL_STM32L1XX)
		set_periph_reg_bit<ADC_CR2, ADC_CR2_DMA>();
#elif defined(HL_STM32F3XX)
		set_periph_reg_bit<ADC_CFGR, ADC_CFGR_DMAEN>();
#endif
	}

	static void disable_dma()
	{
#if defined(HL_STM32L1XX)
		clear_periph_reg_bit<ADC_CR2, ADC_CR2_DMA>();
#elif defined(HL_STM32F3XX)
		clear_periph_reg_bit<ADC_CFGR, ADC_CFGR_DMAEN>();
#endif
	}


	static void set_external_trigger_for_regular_channels(ADCExtTrigger trigger)
	{
#if defined(HL_STM32L1XX)
		set_value_by_mask<uint32_t>(ADC_CR2, ADC_CR2_EXTEN, (uint32_t)trigger << 28);
#elif defined(HL_STM32F3XX)
		set_value_by_mask<uint32_t>(ADC_CFGR, ADC_CFGR_EXTEN, (uint32_t)trigger << 10);
#endif
	}

#if defined(HL_STM32L1XX)
	static void set_external_event_for_regular_group(ADCExtEventRegular event)
	{
		set_value_by_mask<uint32_t>(ADC_CR2, ADC_CR2_EXTSEL, (uint32_t)event);
	}
#elif defined(HL_STM32F3XX)
	static void set_adc12_external_event_for_regular_group(ADC12ExtEventRegular event)
	{
		set_value_by_mask<uint32_t>(ADC_CFGR, ADC_CFGR_EXTSEL, (uint32_t)event);
	}
#endif


#if defined(HL_STM32L1XX)
	static void enable_regular_whatchdog()
	{
		set_periph_reg_bit<ADC_CR1, ADC_CR1_AWDEN>();
	}

	static void disable_regular_whatchdog()
	{
		clear_periph_reg_bit<ADC_CR1, ADC_CR1_AWDEN>();
	}

	static void enable_injected_whatchdog()
	{
		set_periph_reg_bit<ADC_CR1, ADC_CR1_JAWDEN>();
	}

	static void disable_injected_whatchdog()
	{
		clear_periph_reg_bit<ADC_CR1, ADC_CR1_JAWDEN>();
	}

	static void enable_power_down_during_idle()
	{
		set_periph_reg_bit<ADC_CR1, ADC_CR1_PDI>();
	}

	static void disable_power_down_during_idle()
	{
		clear_periph_reg_bit<ADC_CR1, ADC_CR1_PDI>();
	}

	static void enable_power_down_during_delay()
	{
		set_periph_reg_bit<ADC_CR1, ADC_CR1_PDD>();
	}

	static void disable_power_down_during_delay()
	{
		HL_UI32REG(ADC_CR1) &= ~ADC_CR1_PDD;
	}

	static void set_discontinuous_mode_channel_count(const unsigned count)
	{
		set_value_by_mask<uint32_t>(ADC_CR1, ADC_CR1_DISCNUM, (count-1) << 13);
	}

	static void enable_discontinuous_mode_on_injected_channels()
	{
		set_periph_reg_bit<ADC_CR1, ADC_CR1_JDISCEN>();
	}

	static void disable_discontinuous_mode_on_injected_channels()
	{
		clear_periph_reg_bit<ADC_CR1, ADC_CR1_JDISCEN>();
	}

	static void enable_discontinuous_mode_on_regular_channels()
	{
		set_periph_reg_bit<ADC_CR1, ADC_CR1_DISCEN>();
	}

	static void disable_discontinuous_mode_on_regular_channels()
	{
		clear_periph_reg_bit<ADC_CR1, ADC_CR1_DISCEN>();
	}

	static void enable_automatic_injected_group_conversion()
	{
		set_periph_reg_bit<ADC_CR1, ADC_CR1_JAUTO>();
	}

	static void disable_automatic_injected_group_conversion()
	{
		clear_periph_reg_bit<ADC_CR1, ADC_CR1_JAUTO>();
	}

	static void set_watchdog_mode(WatchDogMode mode)
	{
		set_value_by_mask<uint32_t>(ADC_CR1, ADC_CR1_AWDSGL|ADC_CR1_AWDEN|ADC_CR1_JAWDEN, (uint32_t)mode);
	}

	static void enable_interrupt_for_injected_channels()
	{
		set_periph_reg_bit<ADC_CR1, ADC_CR1_JEOCIE>();
	}

	static void disable_interrupt_for_injected_channels()
	{
		clear_periph_reg_bit<ADC_CR1, ADC_CR1_JEOCIE>();
	}

	static void enable_watchdog_interrupt()
	{
		set_periph_reg_bit<ADC_CR1, ADC_CR1_AWDIE>();
	}

	static void disable_watchdog_interrupt()
	{
		clear_periph_reg_bit<ADC_CR1, ADC_CR1_AWDIE>();
	}

	static void enable_eoc_interrupt()
	{
		set_periph_reg_bit<ADC_CR1, ADC_CR1_EOCIE>();
	}

	static void disable_eoc_interrupt()
	{
		clear_periph_reg_bit<ADC_CR1, ADC_CR1_EOCIE>();
	}

	static bool is_eoc_interrupt_enabled()
	{
		return get_periph_reg_bit<ADC_CR1, ADC_CR1_EOCIE>();
	}

	static void set_watchdog_channel(unsigned channel)
	{
		set_value_by_mask<uint32_t>(ADC_CR1, ADC_CR1_AWDCH, channel);
	}

	static void set_external_trigger_for_injected_channels(ADCExtTrigger trigger)
	{
		set_value_by_mask<uint32_t>(ADC_CR2, ADC_CR2_JEXTEN, (uint32_t)trigger << 20);
	}

	static void set_external_event_for_injected_group(ADCExtEventRegular event)
	{
		set_value_by_mask<uint32_t>(ADC_CR2, ADC_CR2_JEXTSEL, (uint32_t)event << 16);
	}

	static void enable_eoc_bit_selection()
	{
		set_periph_reg_bit<ADC_CR2, ADC_CR2_EOCS>();
	}

	static void disable_eoc_bit_selection()
	{
		clear_periph_reg_bit<ADC_CR2, ADC_CR2_EOCS>();
	}

	static void set_delay(ADCDelay delay)
	{
		set_value_by_mask<uint32_t>(ADC_CR2, ADC_CR2_DELS, (uint32_t)delay);
	}

#ifdef ADC_CR2_CFG
	static void select_bank(DMABank bank)
	{
		set_value_by_mask<uint32_t>(ADC_CR2, ADC_CR2_CFG, (uint32_t)bank);
	}
#endif // ADC_CR2_CFG

	static void set_watchdog_higher_threshold(uint16_t value)
	{
		HL_UI32REG(ADC_HTR) = value;
	}

	static void set_watchdog_lower_threshold(uint16_t value)
	{
		HL_UI32REG(ADC_LTR) = value;
	}
#endif

	static void set_regular_len(const unsigned len)
	{
		set_regular_len_internal(len);
#if defined(HL_STM32L1XX)
		while (get_periph_reg_bit<ADC_SR, ADC_SR_RCNR>()) {}
#endif
	}

	template <typename ... Channels>
	static void set_regular_channels(Channels ... channels)
	{
		set_regular_channels_helper<1>(channels ...);
#if defined(HL_STM32L1XX)
		while (get_periph_reg_bit<ADC_SR, ADC_SR_RCNR>()) {}
#endif
	}

	static void set_regular_channel_pos(const unsigned pos, const unsigned channel)
	{
		set_regular_channel_pos_internal(pos, channel);
#if defined(HL_STM32L1XX)
		while (get_periph_reg_bit<ADC_SR, ADC_SR_RCNR>()) {}
#endif
	}

	static bool set_channel_sample_rate(const unsigned channel, const ADCSampleRate rate)
	{
#if defined(HL_STM32L1XX)
		uintptr_t sqr =
				  channel <= 9  ? adc + offsetof(ADC_TypeDef, SMPR3)
				: channel <= 19 ? adc + offsetof(ADC_TypeDef, SMPR2)
				: channel <= 29 ? adc + offsetof(ADC_TypeDef, SMPR1)
				: 0;
#elif defined(HL_STM32F3XX)
		uintptr_t sqr =
				  channel <= 9  ? adc + offsetof(ADC_TypeDef, SMPR1)
				: channel <= 18 ? adc + offsetof(ADC_TypeDef, SMPR2)
				: 0;
#endif
		if (sqr == 0) return false;
		const unsigned shift = 3 * (channel % 10);
		set_value_by_mask<uint32_t>(sqr, 0b111 << shift, ((uint32_t)rate) << shift);
		return true;
	}

	// control

	static void start_regular_conversion()
	{
#if defined(HL_STM32L1XX)
		set_periph_reg_bit<ADC_CR2, ADC_CR2_SWSTART>();
#elif defined(HL_STM32F3XX)
		set_periph_reg_bit<ADC_CR, ADC_CR_ADSTART>();
#endif
	}

	static void stop_regular_conversion()
	{
#if defined(HL_STM32L1XX)
		HL_UI32REG(adc + offsetof(ADC_TypeDef, SMPR1)) = HL_UI32REG(adc + offsetof(ADC_TypeDef, SMPR1));
		set_regular_len_internal(0);
		while (get_periph_reg_bit<ADC_SR, ADC_SR_RCNR>()) {}
#elif defined(HL_STM32F3XX)
		set_periph_reg_bit<ADC_CR, ADC_CR_ADSTP>();
		while (get_periph_reg_bit<ADC_CR, ADC_CR_ADSTP>()) {}
#endif
	}

	static void start_injected_conversion()
	{
#if defined(HL_STM32L1XX)
		set_periph_reg_bit<ADC_CR2, ADC_CR2_JSWSTART>();
#elif defined(HL_STM32F3XX)
#endif
	}

	static uint16_t start_regular_conversion_and_get_value()
	{
		start_regular_conversion();
		while (!get_eoc_flag()) {}
		return get_value();
	}

	static uint16_t get_value()
	{
		return HL_UI32REG(ADC_DR);
	}

	static uintptr_t get_dma_addr()
	{
		return ADC_DR;
	}

private:
	// ADC_TypeDef
	static constexpr uintptr_t adc = detailed::AdcDevice<N>::adc;

#if defined(HL_STM32L1XX)
	static constexpr uintptr_t ADC_SR = adc + offsetof(ADC_TypeDef, SR);
	static constexpr uintptr_t ADC_CR1 = adc + offsetof(ADC_TypeDef, CR1);
	static constexpr uintptr_t ADC_CR2 = adc + offsetof(ADC_TypeDef, CR2);
	static constexpr uintptr_t ADC_HTR = adc + offsetof(ADC_TypeDef, HTR);
	static constexpr uintptr_t ADC_LTR = adc + offsetof(ADC_TypeDef, LTR);

#elif defined(HL_STM32F3XX)
	static constexpr uintptr_t ADC_ISR = adc + offsetof(ADC_TypeDef, ISR);
	static constexpr uintptr_t ADC_CR = adc + offsetof(ADC_TypeDef, CR);
	static constexpr uintptr_t ADC_IER = adc + offsetof(ADC_TypeDef, IER);
	static constexpr uintptr_t ADC_CFGR = adc + offsetof(ADC_TypeDef, CFGR);
#endif
	static constexpr uintptr_t ADC_DR = adc + offsetof(ADC_TypeDef, DR);

	template <uint8_t Pos, typename ... Channels>
	static void set_regular_channels_helper(uint8_t channel, Channels ... other_channels)
	{
		set_regular_channel_pos_internal(Pos, channel);
		set_regular_channels_helper<Pos+1>(other_channels ...);
	}

	template <uint8_t Pos>
	static void set_regular_channels_helper(uint8_t channel)
	{
		set_regular_channel_pos_internal(Pos, channel);

		// TODO: check is Pos allowed value
		set_regular_len_internal(Pos);
	}

	static void set_regular_len_internal(const unsigned len)
	{
#if defined(HL_STM32L1XX)
		constexpr unsigned shift = 20;
		set_value_by_mask<uint32_t>(adc + offsetof(ADC_TypeDef, SQR1), 0b11111 << shift, (len-1) << shift);
#elif defined(HL_STM32F3XX)
		constexpr unsigned shift = 0;
		set_value_by_mask<uint32_t>(adc + offsetof(ADC_TypeDef, SQR1), 0b1111 << shift, (len-1) << shift);
#endif
	}

	static void set_regular_channel_pos_internal(const unsigned pos, const unsigned channel)
	{
#if defined(HL_STM32L1XX)
		uintptr_t sqr =
			  pos <   7 ? adc + offsetof(ADC_TypeDef, SQR5)
			: pos <  13 ? adc + offsetof(ADC_TypeDef, SQR4)
			: pos <  19 ? adc + offsetof(ADC_TypeDef, SQR3)
			: pos <  25 ? adc + offsetof(ADC_TypeDef, SQR2)
			: pos <= 24 ? adc + offsetof(ADC_TypeDef, SQR1)
			: 0;
		const unsigned shift = 5 * ((pos - 1) % 6);
		if (sqr == 0) return;
		set_value_by_mask<uint32_t>(sqr, 0b11111 << shift, channel << shift);
#elif defined(HL_STM32F3XX)
		uintptr_t sqr =
			  pos <=  4 ? adc + offsetof(ADC_TypeDef, SQR1)
			: pos <=  9 ? adc + offsetof(ADC_TypeDef, SQR2)
			: pos <= 14 ? adc + offsetof(ADC_TypeDef, SQR3)
			: pos <= 16 ? adc + offsetof(ADC_TypeDef, SQR4)
			: 0;
		if (sqr == 0) return;
		const unsigned shift = 6 * (pos % 5);
		set_value_by_mask<uint32_t>(sqr, 0b11111 << shift, channel << shift);
#endif
	}
};

///////////////////////////////////////////////////////////////////////////////

#if defined (HL_STM32F3XX)

enum class AdcSyncClock
{
	HCLK_Div1 = 0b01 << 16,
	HCLK_Div2 = 0b10 << 16,
	HCLK_Div4 = 0b11 << 16,
};

enum class AdcAsyncClock
{
	PllDiv1   = 0b10000, // PLL clock divided by 1
	PllDiv2   = 0b10001, // PLL clock divided by 2
	PllDiv4   = 0b10010, // PLL clock divided by 4
	PllDiv6   = 0b10011, // PLL clock divided by 6
	PllDiv8   = 0b10100, // PLL clock divided by 8
	PllDiv10  = 0b10101, // PLL clock divided by 10
	PllDiv12  = 0b10110, // PLL clock divided by 12
	PllDiv16  = 0b10111, // PLL clock divided by 16
	PllDiv32  = 0b11000, // PLL clock divided by 32
	PllDiv64  = 0b11001, // PLL clock divided by 64
	PllDiv128 = 0b11010, // PLL clock divided by 128
	PllDiv256 = 0b11011, // PLL clock divided by 256
};

#endif


template <unsigned N>
class AdcCommon
{
public:
#if defined(HL_STM32L1XX)
	static void adc_enable_temperature_sensor_and_vrefint()
	{
		set_periph_reg_bit<ADC_COMM_CCR, ADC_CCR_TSVREFE>();
		while (!(PWR->CSR & PWR_CSR_VREFINTRDYF)) {}
	}

	static void adc_disable_temperature_sensor_and_vrefint()
	{
		clear_periph_reg_bit<ADC_COMM_CCR, ADC_CCR_TSVREFE>();
	}

#elif defined(HL_STM32F3XX)

	static void enable_vbat_channel()
	{
		set_periph_reg_bit<ADC_COMM_CCR, ADC12_CCR_VBATEN>();
	}

	static void disable_vbat_channel()
	{
		clear_periph_reg_bit<ADC_COMM_CCR, ADC12_CCR_VBATEN>();
	}

	static void enable_temperature_sensor()
	{
		set_periph_reg_bit<ADC_COMM_CCR, ADC12_CCR_TSEN>();
	}

	static void disable_temperature_sensor()
	{
		clear_periph_reg_bit<ADC_COMM_CCR, ADC12_CCR_TSEN>();
	}

	static void enable_vrefint_channel()
	{
		set_periph_reg_bit<ADC_COMM_CCR, ADC12_CCR_VREFEN>();
	}

	static void disable_vrefint_channel()
	{
		clear_periph_reg_bit<ADC_COMM_CCR, ADC12_CCR_VREFEN>();
	}

	static void set_sync_clock(AdcSyncClock clock)
	{
		set_value_by_mask(
			RCC->CFGR2,
			detailed::AdcCommonHelper<N>::async_clock_mask,
			0
		);

		set_value_by_mask<uint32_t>(ADC_COMM_CCR, ADC12_CCR_CKMODE, (uint32_t)clock);
	}

	static void set_async_clock(AdcAsyncClock clock)
	{
		set_value_by_mask<uint32_t>(ADC_COMM_CCR, ADC12_CCR_CKMODE, 0);

		set_value_by_mask(
			RCC->CFGR2,
			detailed::AdcCommonHelper<N>::async_clock_mask,
			(uint32_t)clock << detailed::AdcCommonHelper<N>::async_clock_shift
		);
	}

#endif

private:
	static constexpr uintptr_t adc = detailed::AdcCommonHelper<N>::adc;
	static constexpr uintptr_t ADC_COMM_CCR = adc + offsetof(ADC_Common_TypeDef, CCR);
};

///////////////////////////////////////////////////////////////////////////////

#if defined(HL_STM32L1XX)

#ifdef ADC1
namespace detailed
{
	template <> struct AdcDevice<1>
	{
		static constexpr uint32_t RccBit = RCC_APB2ENR_ADC1EN;
		static constexpr uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2ENR);
		static constexpr uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2RSTR);
		static constexpr uintptr_t adc = ADC1_BASE;
	};
} // namespace detailed
typedef Adc<1> Adc1;
#endif

#ifdef ADC
namespace detailed
{
	template <> struct AdcCommonHelper<1>
	{
		static constexpr uintptr_t adc = ADC_BASE;
	};
}
typedef AdcCommon<1> AdcCommon1;
#endif

#elif defined(HL_STM32F3XX)

///////////////////////////////////////////////////////////////////////////////

#ifdef ADC1
namespace detailed
{
	template <> struct AdcDevice<1>
	{
		static constexpr uint32_t RccBit = RCC_AHBENR_ADC12EN;
		static constexpr uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBENR);
		static constexpr uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBRSTR);
		static constexpr uintptr_t adc = ADC1_BASE;
	};

} // namespace detailed
typedef Adc<1> Adc1;
#endif // ADC1

///////////////////////////////////////////////////////////////////////////////

#ifdef ADC2
namespace detailed
{
	template <> struct AdcDevice<2>
	{
		static constexpr uint32_t RccBit = RCC_AHBENR_ADC12EN;
		static constexpr uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBENR);
		static constexpr uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBRSTR);
		static constexpr uintptr_t adc = ADC2_BASE;
	};
}
typedef Adc<2> Adc2;
#endif // ADC2

///////////////////////////////////////////////////////////////////////////////

#ifdef ADC3
namespace detailed
{
	template <> struct AdcDevice<3>
	{
		static constexpr uint32_t RccBit = RCC_AHBENR_ADC34EN;
		static constexpr uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBENR);
		static constexpr uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBRSTR);
		static constexpr uintptr_t adc = ADC3_BASE;
	};
}
typedef Adc<3> Adc3;
#endif // ADC3

///////////////////////////////////////////////////////////////////////////////

#ifdef ADC4
namespace detailed
{
	template <> struct AdcDevice<4>
	{
		static constexpr uint32_t RccBit = RCC_AHBENR_ADC34EN;
		static constexpr uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBENR);
		static constexpr uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBRSTR);
		static constexpr uintptr_t adc = ADC4_BASE;
	};
}
typedef Adc<4> Adc4;
#endif // ADC4

///////////////////////////////////////////////////////////////////////////////

#ifdef ADC1_2_COMMON
namespace detailed
{
	template <> struct AdcCommonHelper<12>
	{
		// ADC_Common_TypeDef
		static constexpr uintptr_t adc = ADC1_2_COMMON_BASE;
		static constexpr uint32_t async_clock_mask = RCC_CFGR2_ADCPRE12;
		static constexpr unsigned async_clock_shift = 4;
	};
}
typedef AdcCommon<12> AdcCommon12;
#endif // ADC1_2_COMMON

///////////////////////////////////////////////////////////////////////////////

#ifdef ADC3_4_COMMON
namespace detailed
{
	template <> struct AdcCommonHelper<34>
	{
		static constexpr uintptr_t adc = ADC3_4_COMMON_BASE;
		static constexpr uint32_t async_clock_mask = RCC_CFGR2_ADCPRE34;
		static constexpr unsigned async_clock_shift = 9;
	};
}
typedef AdcCommon<34> AdcCommon34;
#endif // ADC3_4_COMMON

#endif // HL_STM32F3XX

} // namespace hl
