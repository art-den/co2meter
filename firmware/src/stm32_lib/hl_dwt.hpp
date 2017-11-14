#pragma once

#include <stdint.h>
#include "hl_device.hpp"

namespace hl {

class DWTCounter
{
public:
	static void enable()
	{
		HL_UI32REG(SCB_DEMCR)  |= 0x01000000;
		HL_UI32REG(DWT_CYCCNT) = 0;
		HL_UI32REG(DWT_CONTROL) |= 1;
	}

	inline static uint32_t get()
	{
		return HL_UI32REG(DWT_CYCCNT);
	}

private:
	static constexpr uintptr_t DWT_CYCCNT  = 0xE0001004;
	static constexpr uintptr_t DWT_CONTROL = 0xE0001000;
	static constexpr uintptr_t SCB_DEMCR   = 0xE000EDFC;
};

template <unsigned SysFreq>
class Delay
{
public:
	inline static void exec_ms(uint32_t value)
	{
		exec_ticks(value * (SysFreq/1000));
	}

	inline static void exec_us(uint32_t value)
	{
		exec_ticks(value * (SysFreq/1000000));
	}

	inline static void exec_ns(uint32_t value)
	{
		exec_ticks(value * SysFreq / 1000000000);
	}


	inline static void exec_ms_with_sleep(uint32_t value)
	{
		exec_ticks_wfi(value * (SysFreq/1000));
	}

	inline static void exec_us_with_sleep(uint32_t value)
	{
		exec_ticks_wfi(value * (SysFreq/1000000));
	}


private:
	inline static void exec_ticks(uint32_t ticks)
	{
		int32_t stop = DWTCounter::get()+ticks;
		while ((int32_t)(DWTCounter::get()-stop) < 0) {}
	}

	inline static void exec_ticks_wfi(uint32_t ticks)
	{
		int32_t stop = DWTCounter::get()+ticks;
		while ((int32_t)(DWTCounter::get()-stop) < 0) __WFI();
	}
};


} // namespace hl
