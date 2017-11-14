#pragma once

#include <stm32l1xx.h>
#include <stdint.h>

namespace hl {

enum class RTCHourFormat
{
	AMor24 = 0 << 22, // AM or 24-hour format
	PM     = 1 << 22, // PM
};

struct RTCTime
{
	uint8_t hours = 0;
	uint8_t minutes = 0;
	uint8_t seconds = 0;

	RTCTime() = default;

	RTCTime(uint8_t hours, uint8_t minutes, uint8_t seconds) :
		hours(hours),
		minutes(minutes),
		seconds(seconds)
	{}
};

struct RTCDate
{
	uint16_t years = 1;
	uint8_t  mon = 1;
	uint8_t  day = 1;

	RTCDate() = default;

	RTCDate(uint16_t years, uint8_t mon, uint8_t day) :
		years(years),
		mon(mon),
		day(day)
	{}
};

namespace detailed {

inline void rtc_disable_write_protection()
{
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
}

inline void rtc_enable_write_protection()
{
	RTC->WPR = 0xFF;
}

inline void rtc_enter_dt_init_mode()
{
	RTC->ISR |= RTC_ISR_INIT;
	while (!(RTC->ISR & RTC_ISR_INITF)) {}
}

inline void rtc_leave_dt_init_mode()
{
	RTC->ISR &= ~RTC_ISR_INIT;
	while (RTC->ISR & RTC_ISR_INITF) {}
}

inline void rtc_set_bcd_value (uint8_t value, unsigned tens_shift, unsigned units_shift, uint32_t &dst)
{
	const unsigned tens = value / 10;
	const unsigned units = value % 10;
	dst |= (tens << tens_shift);
	dst |= (units << units_shift);
}

inline uint8_t rtc_get_bcd_value (uint32_t value, unsigned mask, unsigned tens_shift, unsigned units_shift)
{
	value &= mask;
	unsigned tens = (value >> tens_shift) & 0xF;
	unsigned units = (value >> units_shift) & 0xF;
	return tens * 10 + units;
}



} // end 'namespace detailed'

inline bool rtc_is_calendar_initialized()
{
	return RTC->ISR & RTC_ISR_INITS;
}


// calendar

inline void rtc_init_date_and_time(
	RTCHourFormat hour_format,
	const RTCDate &date,
	const RTCTime &time)
{
	detailed::rtc_disable_write_protection();
	detailed::rtc_enter_dt_init_mode();
	uint32_t tr = RTC->TR;
	uint32_t dr = RTC->DR;

	tr &= ~(RTC_TR_PM|RTC_TR_HT|RTC_TR_HU|RTC_TR_MNT|RTC_TR_MNU|RTC_TR_ST|RTC_TR_SU);
	dr &= ~(RTC_DR_YT|RTC_DR_YU|RTC_DR_MT|RTC_DR_MU|RTC_DR_DT|RTC_DR_DU);

	tr |= (uint32_t)hour_format;

	detailed::rtc_set_bcd_value(date.years % 2000, 20, 16, dr);
	detailed::rtc_set_bcd_value(date.mon,          12,  8, dr);
	detailed::rtc_set_bcd_value(date.day,           4,  0, dr);
	detailed::rtc_set_bcd_value(time.hours,        20, 16, tr);
	detailed::rtc_set_bcd_value(time.minutes,      12,  8, tr);
	detailed::rtc_set_bcd_value(time.seconds,       4,  0, tr);

	RTC->TR = tr;
	RTC->DR = dr;
	detailed::rtc_leave_dt_init_mode();
	detailed::rtc_enable_write_protection();
}

inline void rtc_get_date(RTCDate &date)
{
	while (!(RTC->ISR & RTC_ISR_RSF)) {}
	uint32_t dr = RTC->DR;
	date.years = detailed::rtc_get_bcd_value(dr, RTC_DR_YT|RTC_DR_YU, 20, 16);
	date.mon   = detailed::rtc_get_bcd_value(dr, RTC_DR_MT|RTC_DR_MU, 12,  8);
	date.day   = detailed::rtc_get_bcd_value(dr, RTC_DR_DT|RTC_DR_DU,  4,  0);
}

inline void rtc_get_time(RTCTime &time)
{
	while (!(RTC->ISR & RTC_ISR_RSF)) {}
	uint32_t tr = RTC->TR;
	time.hours   = detailed::rtc_get_bcd_value(tr, RTC_TR_HT|RTC_TR_HU,   20, 16);
	time.minutes = detailed::rtc_get_bcd_value(tr, RTC_TR_MNT|RTC_TR_MNU, 12,  8);
	time.seconds = detailed::rtc_get_bcd_value(tr, RTC_TR_ST|RTC_TR_SU,    4,  0);
}


// Prescaler

inline void rtc_set_prescaler(uint8_t asynchronous, uint16_t synchronous)
{
	detailed::rtc_disable_write_protection();
	detailed::rtc_enter_dt_init_mode();

	uint32_t prer = RTC->PRER;
	prer &= RTC_PRER_PREDIV_S;
	prer |= synchronous;
	RTC->PRER = prer;

	prer |= ((uint32_t)asynchronous << 16);
	RTC->PRER = prer;

	detailed::rtc_leave_dt_init_mode();
	detailed::rtc_enable_write_protection();
}


// wakeup timer

enum class RTCWUClock
{
	RTC_16   = 0b000 << 0, // RTC/16 clock is selected
	RTC_8    = 0b001 << 0, // RTC/8 clock is selected
	RTC_4    = 0b010 << 0, // RTC/4 clock is selected
	RTC_2    = 0b011 << 0, // RTC/2 clock is selected
	CK_SPRE1 = 0b100 << 0, // ck_spre (usually 1 Hz) clock is selected
	CK_SPRE2 = 0b110 << 0, // ck_spre (usually 1 Hz) clock is selected and 2^16 is added to the WUT counter value(see note below)
};

enum class RtcWakeUpInterrupt
{
	Disabled = 0,
	Enabled = RTC_CR_WUTIE,
};

inline void rtc_configure_wakeup_timer(
	RTCWUClock         clock,
	uint16_t           period,
	RtcWakeUpInterrupt interrupt)
{
	detailed::rtc_disable_write_protection();
	detailed::rtc_enter_dt_init_mode();

	RTC->CR &= ~RTC_CR_WUTE;
	while (!(RTC->ISR & RTC_ISR_WUTWF)) {}

	uint32_t cr = RTC->CR;
	uint32_t wutr = RTC->WUTR;

	cr &= ~(RTC_CR_WUCKSEL);
	wutr &= ~RTC_WUTR_WUT;

	cr |= (uint32_t)clock;
	cr |= (uint32_t)interrupt;

	wutr |= period;

	RTC->WUTR = wutr;
	RTC->CR = cr;

	RTC->CR |= RTC_CR_WUTE;

	detailed::rtc_leave_dt_init_mode();
	detailed::rtc_enable_write_protection();
}

inline bool rtc_get_wakeup_flag()
{
	return (RTC->ISR & RTC_ISR_WUTF);
}

inline void rtc_clear_wakeup_flag()
{
	RTC->ISR = ~RTC_ISR_WUTF;
}


} // namespace hl
