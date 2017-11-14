#pragma once

#include "hl_device.hpp"

#if defined (HL_STM32L1XX)
	#define HL_GPIO_VER2
#elif defined (HL_STM32F1XX)
	#define HL_GPIO_VER1
#elif defined (HL_STM32F3XX)
	#define HL_GPIO_VER2
#else
	#error "You must define HL_STM32L1XX or HL_STM32F3XX"
#endif

namespace hl {

enum class AltFun
{
	System        = 0,
#ifdef HL_STM32L1XX
	Timer2        = 1,
	Timers3_4_5   = 2,
	Timers9_10_11 = 3,
	I2C           = 4,
	SPI_1_2       = 5,
	SPI_3         = 6,
	USART         = 7,
	USB_          = 10,
	LCD_          = 11,
	RI_           = 14,
	EventOut      = 15
#endif

#ifdef HL_STM32F3XX
#endif
};


template<class PortHelper>
class Port : public Device<PortHelper>
{
public:
	typedef PortHelper Helper;

	static void HL_ALWAYS_INLINE set(uint16_t value)
	{
		((volatile GPIO_TypeDef*)Helper::gpio)->ODR = value;
	}

	static void HL_ALWAYS_INLINE set(uint16_t value, uint16_t mask)
	{
		HL_UI32REG(BSRR) = value | ((uint32_t)mask << 16);
	}

	static uint16_t HL_ALWAYS_INLINE get_in()
	{
		return ((volatile GPIO_TypeDef*)Helper::gpio)->IDR;
	}

	static uint16_t HL_ALWAYS_INLINE get_out()
	{
		return ((volatile GPIO_TypeDef*)Helper::gpio)->ODR;
	}

public:
#if defined (HL_GPIO_VER1)
	constexpr static uintptr_t BSRR = Helper::gpio + 0x10;
#elif defined (HL_GPIO_VER2)
	constexpr static uintptr_t BSRR = Helper::gpio + 0x18;
#endif
};

enum class PinSpeed
{
#if defined (HL_GPIO_VER1)
	Low     = 0b10,
	Medium  = 0b01,
	High    = 0b11,
#elif defined (HL_GPIO_VER2)
	VeryLow = 0b00,
	Low     = 0b01,
	Medium  = 0b10,
	High    = 0b11,
#endif
};

template <class PortType, unsigned PinIndex>
class Pin
{
public:
	static constexpr unsigned Index = PinIndex;
	typedef PortType Port;

	// digital input

	static HL_ALWAYS_INLINE void conf_in()
	{
#if defined (HL_GPIO_VER1)
		set_value_by_mask<uint32_t>(CR, InvMask4, 0b01/*float in*/ << (Shift4+2));
#elif defined (HL_GPIO_VER2)
		auto *gpio = (GPIO_TypeDef*)PortType::Helper::gpio;
		gpio->MODER = (gpio->MODER & InvMask2) | (ModerInput << PinIndex*2);
		gpio->PUPDR = (gpio->PUPDR & InvMask2);
#endif
	}

	static void HL_ALWAYS_INLINE conf_in_pull_up()
	{
#if defined (HL_GPIO_VER1)
		set_value_by_mask<uint32_t>(CR, InvMask4, 0b10/*pulled in*/ << (Shift4+2));
		on();
#elif defined (HL_GPIO_VER2)
		auto *gpio = (GPIO_TypeDef*)PortType::Helper::gpio;
		gpio->MODER = (gpio->MODER & InvMask2) | (ModerInput << PinIndex*2);
		gpio->PUPDR = (gpio->PUPDR & InvMask2) | (PupdrPullUp << PinIndex*2);
#endif
	}

	static void HL_ALWAYS_INLINE conf_in_pull_down()
	{
#if defined (HL_GPIO_VER1)
		set_value_by_mask<uint32_t>(CR, InvMask4, 0b10/*pulled in*/ << (Shift4+2));
		off();
#elif defined (HL_GPIO_VER2)
		auto *gpio = (GPIO_TypeDef*)PortType::Helper::gpio;
		gpio->MODER = (gpio->MODER & InvMask2) | (ModerInput << PinIndex*2);
		gpio->PUPDR = (gpio->PUPDR & InvMask2) | (PupdrPullDown << PinIndex*2);
#endif
	}

	static bool HL_ALWAYS_INLINE get_in()
	{
		return get_periph_reg_bit<IDR, BitMask>();
	}

	static void HL_ALWAYS_INLINE configure_exti(bool rising, bool falling, bool enable_event, bool enable_interrupt)
	{
#if defined (HL_GPIO_VER1)
#elif defined (HL_GPIO_VER2)
		constexpr uint32_t Shift = (PinIndex & 0x3) * 4;
		constexpr uint32_t Mask  = (0xF << Shift);
		SYSCFG->EXTICR[PinIndex >> 2] = (SYSCFG->EXTICR[PinIndex >> 2] & Mask) | (PortType::Helper::EXTIx << Shift);
		EXTI->EMR = (EXTI->EMR & InvMask1) | (enable_event ? BitMask : 0);
		EXTI->IMR = (EXTI->IMR & InvMask1) | (enable_interrupt ? BitMask : 0);
	    EXTI->RTSR = (EXTI->RTSR & InvMask1) | (rising ? BitMask : 0);
	    EXTI->FTSR = (EXTI->FTSR & InvMask1) | (falling ? BitMask : 0);
#endif
	}

	static bool HL_ALWAYS_INLINE get_interrupt_bit()
	{
		return EXTI->PR & BitMask;
	}

	static void HL_ALWAYS_INLINE clear_interrupt_bit()
	{
		EXTI->PR = BitMask;
	}


	// digital output

	static void HL_ALWAYS_INLINE conf_out_push_pull(PinSpeed speed = PinSpeed::High)
	{
#if defined (HL_GPIO_VER1)
		set_value_by_mask<uint32_t>(CR, InvMask4, ((uint32_t)speed << Shift4) | (0b00/*pp*/ << (Shift4+2)));
#elif defined (HL_GPIO_VER2)
		auto *gpio = (GPIO_TypeDef*)PortType::Helper::gpio;
		gpio->MODER = (gpio->MODER & InvMask2) | (ModerOutput << PinIndex*2);
		gpio->OTYPER = (gpio->OTYPER & InvMask1) | (OyperPP << PinIndex);
		gpio->OSPEEDR = (gpio->OSPEEDR & InvMask2) | (((uint32_t)speed) << PinIndex*2);
#endif
	}

	static void HL_ALWAYS_INLINE conf_out_open_drain(PinSpeed speed = PinSpeed::High)
	{
#if defined (HL_GPIO_VER1)
		set_value_by_mask<uint32_t>(CR, InvMask4, ((uint32_t)speed << Shift4) | (0b01/*od*/ << (Shift4+2)));
#elif defined (HL_GPIO_VER2)
		auto *gpio = (GPIO_TypeDef*)PortType::Helper::gpio;
		gpio->MODER = (gpio->MODER & InvMask2) | (ModerOutput << PinIndex*2);
		gpio->OTYPER = (gpio->OTYPER & InvMask1) | (OyperOD << PinIndex);
		gpio->OSPEEDR = (gpio->OSPEEDR & InvMask2) | (((uint32_t)speed) << PinIndex*2);
#endif
	}

	static void HL_ALWAYS_INLINE on()
	{
		HL_UI32REG(Port::BSRR) = BitMask;
	}

	static void HL_ALWAYS_INLINE toggle()
	{
		set_out(!get_out());
	}

	static void HL_ALWAYS_INLINE off()
	{
		HL_UI32REG(Port::BSRR) = (BitMask << 16);
	}

	static void HL_ALWAYS_INLINE set_out(bool const value)
	{
		if (value) on();
		else off();
	}

	static bool HL_ALWAYS_INLINE get_out()
	{
		return get_periph_reg_bit<ODR, BitMask>();
	}

	// alternate function

	static void HL_ALWAYS_INLINE conf_alt_push_pull(PinSpeed speed = PinSpeed::High)
	{
#if defined (HL_GPIO_VER1)
		set_value_by_mask<uint32_t>(CR, InvMask4, ((uint32_t)speed << Shift4) | (0b10/*alt pp*/ << (Shift4+2)));
#elif defined (HL_GPIO_VER2)
		auto *gpio = (GPIO_TypeDef*)PortType::Helper::gpio;
		gpio->MODER = (gpio->MODER & InvMask2) | (ModerAF << PinIndex*2);
		gpio->OTYPER = (gpio->OTYPER & InvMask1) | (OyperPP << PinIndex);
		gpio->OSPEEDR = (gpio->OSPEEDR & InvMask2) | (((uint32_t)speed) << PinIndex*2);
#endif
	}

	static void HL_ALWAYS_INLINE conf_alt_open_drain(PinSpeed speed = PinSpeed::High)
	{
#if defined (HL_GPIO_VER1)
		set_value_by_mask<uint32_t>(CR, InvMask4, ((uint32_t)speed << Shift4) | (0b11/*alt od*/ << (Shift4+2)));
#elif defined (HL_GPIO_VER2)
		auto *gpio = (GPIO_TypeDef*)PortType::Helper::gpio;
		gpio->MODER = (gpio->MODER & InvMask2) | (ModerAF << PinIndex*2);
		gpio->OTYPER = (gpio->OTYPER & InvMask1) | (OyperOD << PinIndex);
		gpio->OSPEEDR = (gpio->OSPEEDR & InvMask2) | (((uint32_t)speed) << PinIndex*2);
#endif
	}

#if defined (HL_GPIO_VER2)
	static void HL_ALWAYS_INLINE set_alt_function(AltFun func)
	{
		constexpr uint32_t AfrIndex = PinIndex >> 3;
		constexpr uint32_t Shift = (PinIndex & 0x7) * 4;
		constexpr uint32_t Mask = ~(0xF << Shift);
		auto *gpio = (GPIO_TypeDef*)PortType::Helper::gpio;
		gpio->AFR[AfrIndex] = (gpio->AFR[AfrIndex] & Mask) | ((uint32_t)func << Shift);
	}
#endif

	// analog

	static void HL_ALWAYS_INLINE conf_analog()
	{
#if defined (HL_GPIO_VER1)
		set_value_by_mask<uint32_t>(CR, InvMask4, (0b00/*analog in*/ << (Shift4+2)));
#elif defined (HL_GPIO_VER2)
		auto *gpio = (GPIO_TypeDef*)PortType::Helper::gpio;
		gpio->MODER = (gpio->MODER & InvMask2) | (ModerAnalog << PinIndex*2);
		gpio->PUPDR = (gpio->PUPDR & InvMask2);
#endif
	}

private:
//	constexpr static GPIO_TypeDef *gpio = PortType::Helper::gpio;
	constexpr static uint32_t BitMask = (1U << PinIndex);
#if defined (HL_GPIO_VER1)
	constexpr static uintptr_t CR = PortType::Helper::gpio + ((PinIndex < 8) ? offsetof(GPIO_TypeDef, CRL) : offsetof(GPIO_TypeDef, CRH));

	constexpr static uint32_t Shift4 = (PinIndex % 8)*4;
	constexpr static uint32_t InvMask4 = (uint32_t)0b1111 << Shift4;

#elif defined (HL_GPIO_VER2)
	constexpr static uint32_t InvMask2 = ~(3U << PinIndex*2);
	constexpr static uint32_t InvMask1 = ~BitMask;
	constexpr static uint32_t ModerInput  = 0;
	constexpr static uint32_t ModerOutput = 1;
	constexpr static uint32_t ModerAF     = 2;
	constexpr static uint32_t ModerAnalog = 3;
	constexpr static uint32_t PupdrNone = 0;
	constexpr static uint32_t PupdrPullUp = 1;
	constexpr static uint32_t PupdrPullDown = 2;
	constexpr static uint32_t OyperPP = 0;
	constexpr static uint32_t OyperOD = 1;
#endif
	constexpr static uintptr_t IDR = PortType::Helper::gpio + offsetof(GPIO_TypeDef, IDR);
	constexpr static uintptr_t ODR = PortType::Helper::gpio + offsetof(GPIO_TypeDef, ODR);
};

#define HL_DECL_PINS(PORT) \
typedef Pin<PORT,  0> PORT##0; \
typedef Pin<PORT,  1> PORT##1; \
typedef Pin<PORT,  2> PORT##2; \
typedef Pin<PORT,  3> PORT##3; \
typedef Pin<PORT,  4> PORT##4; \
typedef Pin<PORT,  5> PORT##5; \
typedef Pin<PORT,  6> PORT##6; \
typedef Pin<PORT,  7> PORT##7; \
typedef Pin<PORT,  8> PORT##8; \
typedef Pin<PORT,  9> PORT##9; \
typedef Pin<PORT, 10> PORT##10; \
typedef Pin<PORT, 11> PORT##11; \
typedef Pin<PORT, 12> PORT##12; \
typedef Pin<PORT, 13> PORT##13; \
typedef Pin<PORT, 14> PORT##14; \
typedef Pin<PORT, 15> PORT##15;

#if defined(RCC_APB2ENR_IOPAEN) || defined(RCC_AHBENR_GPIOAEN)
namespace detailed { struct PortHelperA
{
#if defined (HL_GPIO_VER1)
	constexpr static uint32_t RccBit = RCC_APB2ENR_IOPAEN;
	constexpr static uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2RSTR);
	constexpr static uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2ENR);
#elif defined (HL_GPIO_VER2)
	constexpr static uint32_t RccBit = RCC_AHBENR_GPIOAEN;
	constexpr static uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBRSTR);
	constexpr static uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBENR);
#endif
	constexpr static uintptr_t gpio = GPIOA_BASE;
	constexpr static uint32_t EXTIx = 0;
}; }
typedef Port<detailed::PortHelperA> PA;
HL_DECL_PINS(PA)
#endif

#if defined(RCC_APB2ENR_IOPBEN) || defined(RCC_AHBENR_GPIOBEN)
namespace detailed { struct PortHelperB
{
#if defined (HL_GPIO_VER1)
	constexpr static uint32_t RccBit = RCC_APB2ENR_IOPBEN;
	constexpr static uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2RSTR);
	constexpr static uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2ENR);
#elif defined (HL_GPIO_VER2)
	constexpr static uint32_t RccBit = RCC_AHBENR_GPIOBEN;
	constexpr static uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBENR);
	constexpr static uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBRSTR);
#endif
	constexpr static uintptr_t gpio = GPIOB_BASE;
	constexpr static uint32_t EXTIx = 1;
}; }
typedef Port<detailed::PortHelperB> PB;
HL_DECL_PINS(PB)
#endif

#if defined(RCC_APB2ENR_IOPCEN) || defined(RCC_AHBENR_GPIOCEN)
namespace detailed { struct PortHelperC
{
#if defined (HL_GPIO_VER1)
	constexpr static uint32_t RccBit = RCC_APB2ENR_IOPCEN;
	constexpr static uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2RSTR);
	constexpr static uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2ENR);
#elif defined (HL_GPIO_VER2)
	constexpr static uint32_t RccBit = RCC_AHBENR_GPIOCEN;
	constexpr static uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBENR);
	constexpr static uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBRSTR);
#endif
	constexpr static uintptr_t gpio = GPIOC_BASE;
	constexpr static uint32_t EXTIx = 2;
}; }
typedef Port<detailed::PortHelperC> PC;
HL_DECL_PINS(PC)
#endif

#if defined(RCC_APB2ENR_IOPDEN) || defined(RCC_AHBENR_GPIODEN)
namespace detailed { struct PortHelperD
{
#if defined (HL_GPIO_VER1)
	constexpr static uint32_t RccBit = RCC_APB2ENR_IOPDEN;
	constexpr static uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2RSTR);
	constexpr static uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2ENR);
#elif defined (HL_GPIO_VER2)
	constexpr static uint32_t RccBit = RCC_AHBENR_GPIODEN;
	constexpr static uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBENR);
	constexpr static uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBRSTR);
#endif
	constexpr static uintptr_t gpio = GPIOD_BASE;
	constexpr static uint32_t EXTIx = 3;
}; }
typedef Port<detailed::PortHelperD> PD;
HL_DECL_PINS(PD)
#endif

#if defined(RCC_APB2ENR_IOPEEN) || defined(RCC_AHBENR_GPIOEEN)
namespace detailed { struct PortHelperE
{
#if defined (HL_GPIO_VER1)
	constexpr static uint32_t RccBit = RCC_APB2ENR_IOPEEN;
	constexpr static uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2RSTR);
	constexpr static uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2ENR);
#elif defined (HL_GPIO_VER2)
	constexpr static uint32_t RccBit = RCC_AHBENR_GPIOEEN;
	constexpr static uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBENR);
	constexpr static uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBRSTR);
#endif
	constexpr static uintptr_t gpio = GPIOE_BASE;
	constexpr static uint32_t EXTIx = 4;
}; }
typedef Port<detailed::PortHelperE> PE;
HL_DECL_PINS(PE)
#endif

#if defined(RCC_APB2ENR_IOPFEN) || defined(RCC_AHBENR_GPIOFEN)
namespace detailed { struct PortHelperF
{
#if defined (HL_GPIO_VER1)
	constexpr static uint32_t RccBit = RCC_APB2ENR_IOPFEN;
	constexpr static uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2RSTR);
	constexpr static uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2ENR);
#elif defined (HL_GPIO_VER2)
	constexpr static uint32_t RccBit = RCC_AHBENR_GPIOFEN;
	constexpr static uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBENR);
	constexpr static uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBRSTR);
#endif
	constexpr static uintptr_t gpio = GPIOF_BASE;
	constexpr static uint32_t EXTIx = 5;
}; }
typedef Port<detailed::PortHelperF> PF;
HL_DECL_PINS(PF)
#endif

#if defined(RCC_APB2ENR_IOPGEN) || defined(RCC_AHBENR_GPIOGEN)
namespace detailed { struct PortHelperG
{
#if defined (HL_GPIO_VER1)
	constexpr static uint32_t RccBit = RCC_APB2ENR_IOPGEN;
	constexpr static uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2RSTR);
	constexpr static uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, APB2ENR);
#elif defined (HL_GPIO_VER2)
	constexpr static uint32_t RccBit = RCC_AHBENR_GPIOGEN;
	constexpr static uintptr_t ClockRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBENR);
	constexpr static uintptr_t ResetRegister = RCC_BASE + offsetof(RCC_TypeDef, AHBRSTR);
#endif
	constexpr static uintptr_t gpio = GPIOG_BASE;
	constexpr static uint32_t EXTIx = 6;
}; }
typedef Port<detailed::PortHelperG> PG;
HL_DECL_PINS(PG)
#endif
#undef HL_DECL_PINS

// remap function for STM32F1 devices
#if defined (HL_GPIO_VER1)

enum class SWJTAGConf
{
	Full            = 0b00 << 24,
	FullWithoutNRTS = 0b01 << 24,
	SeralWire       = 0b10 << 24,
	Disabled        = 0b11 << 24,
};

inline void gpio_set_serial_wire_jtag_conf(SWJTAGConf value)
{
	set_value_by_mask(AFIO->MAPR, AFIO_MAPR_SWJ_CFG, (uint32_t)value);
}

#endif

} // namespace hl

