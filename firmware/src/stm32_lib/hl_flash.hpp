#pragma once

#include "hl_device.hpp"
#include <stdint.h>
#include <stddef.h>

namespace hl
{

//#if defined(HL_STM32F1XX)

//#endif

#if defined(HL_STM32F3XX) || defined(HL_STM32F1XX)

inline void flash_enable_prefetch()
{
	FLASH->ACR |= FLASH_ACR_PRFTBE;
	while (!(FLASH->ACR & FLASH_ACR_PRFTBS)) {}
}

inline void flash_disable_prefetch()
{
	FLASH->ACR &= ~FLASH_ACR_PRFTBE;
}

enum class FlashLatency
{
	_0 = 0,
	_1 = FLASH_ACR_LATENCY_0,
	_2 = FLASH_ACR_LATENCY_1,
};

inline void flash_set_latency(FlashLatency latency)
{
	set_value_by_mask(FLASH->ACR, FLASH_ACR_LATENCY, (uint32_t)latency);
}

#endif

#if defined(HL_STM32L1XX)

// power saving during run

inline void flash_enable_power_saving_during_run()
{
	FLASH->ACR |= FLASH_ACR_RUN_PD;
}

inline void flash_disable_power_saving_during_run()
{
	FLASH->ACR &= ~FLASH_ACR_RUN_PD;
}


// power saving during sleep

inline void flash_enable_power_saving_during_sleep()
{
	FLASH->ACR |= FLASH_ACR_SLEEP_PD;
}

inline void flash_disable_power_saving_during_sleep()
{
	FLASH->ACR &= ~FLASH_ACR_SLEEP_PD;
}


// 64 bit access

inline void flash_enable_64bit_access()
{
	FLASH->ACR |= FLASH_ACR_ACC64;
	while (!(FLASH->ACR & FLASH_ACR_ACC64)) {}
}

inline void flash_disable_64bit_access()
{
	FLASH->ACR &= ~FLASH_ACR_ACC64;
	while (FLASH->ACR & FLASH_ACR_ACC64) {}
}


// prefetch

inline void flash_enable_prefetch()
{
	FLASH->ACR |= FLASH_ACR_PRFTEN;
	while (!(FLASH->ACR & FLASH_ACR_PRFTEN)) {}
}

inline void flash_disable_prefetch()
{
	FLASH->ACR &= ~FLASH_ACR_PRFTEN;
	while (FLASH->ACR & FLASH_ACR_PRFTEN) {}
}


// wait state (latency)

inline void flash_set_set_zero_wait_state()
{
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	while (FLASH->ACR & FLASH_ACR_LATENCY) {}
}

inline void flash_set_one_wait_state()
{
	FLASH->ACR |= FLASH_ACR_LATENCY;
	while (!(FLASH->ACR & FLASH_ACR_LATENCY)) {}
}


// EEPROM

constexpr uintptr_t EepromStartAddr = 0x08080000;

inline void eeprom_unlock()
{
	if (FLASH->PECR & FLASH_PECR_PELOCK)
	{
		FLASH->PEKEYR = 0x89ABCDEF;
		FLASH->PEKEYR = 0x02030405;
		while (FLASH->SR & FLASH_SR_BSY) {}
	}
}

inline void eeprom_lock()
{
	FLASH->PECR |= FLASH_PECR_PELOCK;
}


class EepromUnlocker
{
public:
	EepromUnlocker()
	{
		auto &cnt = counter();
		if (cnt == 0) eeprom_unlock();
		cnt++;
	}

	~EepromUnlocker()
	{
		auto &cnt = counter();
		cnt--;
		if (cnt == 0) eeprom_lock();
	}

private:
	unsigned& counter()
	{
		static unsigned counter = 0;
		return counter;
	}
};


inline void eeprom_erase_word(uintptr_t dst)
{
	*(volatile uint32_t*)dst = 0;
	while (FLASH->SR & FLASH_SR_BSY) {}
}


inline void eeprom_fast_write_int32(uintptr_t dst, uint32_t data, bool *eeprom_unlocked_flag = nullptr)
{
	if (*(volatile uint32_t*)dst == data) return;
	if (eeprom_unlocked_flag && !*eeprom_unlocked_flag)
	{
		eeprom_unlock();
		*eeprom_unlocked_flag = true;
	}
	FLASH->PECR &= ~FLASH_PECR_FTDW;
	*(volatile uint32_t*)dst = data;
	while (FLASH->SR & FLASH_SR_BSY) {}
}


inline void eeprom_write_int32(uintptr_t dst, uint32_t data, bool *eeprom_unlocked_flag = nullptr)
{
	if (*(volatile uint32_t*)dst == data) return;
	if (eeprom_unlocked_flag && !*eeprom_unlocked_flag)
	{
		eeprom_unlock();
		*eeprom_unlocked_flag = true;
	}
	FLASH->PECR |= FLASH_PECR_FTDW;
	*(volatile uint32_t*)dst = data;
	while (FLASH->SR & FLASH_SR_BSY) {}
}


template <typename T>
void write_null_data(uintptr_t dst)
{
	volatile uint32_t *addr = (volatile uint32_t*)((uintptr_t)dst & ~3UL);
	unsigned offset = (unsigned)dst & 3UL;
	uint32_t value = *addr;
	uint32_t mask = (T)(~T());
	mask <<= offset * 8;
	value &= ~mask;
	*addr = value;
}


inline void eeprom_fast_write_int16(uintptr_t dst, uint16_t data, bool *eeprom_unlocked_flag = nullptr)
{
	if (*(volatile uint16_t*)dst == data) return;
	if (eeprom_unlocked_flag && !*eeprom_unlocked_flag)
	{
		eeprom_unlock();
		*eeprom_unlocked_flag = true;
	}
	FLASH->PECR &= ~FLASH_PECR_FTDW;
	*(volatile uint16_t*)dst = data;
	while (FLASH->SR & FLASH_SR_BSY) {}
}


inline void eeprom_write_int16(uintptr_t dst, uint16_t data, bool *eeprom_unlocked_flag = nullptr)
{
	if (*(volatile uint16_t*)dst == data) return;
	if (eeprom_unlocked_flag && !*eeprom_unlocked_flag)
	{
		eeprom_unlock();
		*eeprom_unlocked_flag = true;
	}
	FLASH->PECR |= FLASH_PECR_FTDW;
	if (data != 0)
		*(volatile uint16_t*)dst = data;
	else
		write_null_data<uint16_t>(dst);
	while (FLASH->SR & FLASH_SR_BSY) {}
}


inline void eeprom_fast_write_int8(uintptr_t dst, uint8_t data, bool *eeprom_unlocked_flag = nullptr)
{
	if (*(volatile uint8_t*)dst == data) return;
	if (eeprom_unlocked_flag && !*eeprom_unlocked_flag)
	{
		eeprom_unlock();
		*eeprom_unlocked_flag = true;
	}
	FLASH->PECR &= ~FLASH_PECR_FTDW;
	*(volatile uint8_t*)dst = data;
	while (FLASH->SR & FLASH_SR_BSY) {}
}


inline void eeprom_write_int8(uintptr_t dst, uint8_t data, bool *eeprom_unlocked_flag = nullptr)
{
	if (*(volatile uint8_t*)dst == data) return;
	if (eeprom_unlocked_flag && !*eeprom_unlocked_flag)
	{
		eeprom_unlock();
		*eeprom_unlocked_flag = true;
	}
	FLASH->PECR |= FLASH_PECR_FTDW;
	if (data != 0)
		*(volatile uint8_t*)dst = data;
	else
		write_null_data<uint8_t>(dst);
	while (FLASH->SR & FLASH_SR_BSY) {}
}


inline void eeprom_write_data(const volatile uint8_t *src, uintptr_t dst, size_t len)
{
	bool eeprom_unlocked = false;

	while (len != 0)
	{
		size_t item_len = 0;
		if (len >= 4)
		{
			eeprom_write_int32(dst, *(uint32_t*)src, &eeprom_unlocked);
			item_len = 4;
		}
		else if (len >= 2)
		{
			eeprom_write_int16(dst, *(uint16_t*)src, &eeprom_unlocked);
			item_len = 2;
		}
		else
		{
			eeprom_write_int8(dst, *(uint8_t*)src, &eeprom_unlocked);
			item_len = 1;
		}

		len -= item_len;
		dst += item_len;
		src += item_len;
	}

	if (eeprom_unlocked) eeprom_lock();
}


inline void eeprom_read_data(uintptr_t src, volatile uint8_t *dst, size_t len)
{
	while (len != 0)
	{
		size_t item_len = 0;
		if (len >= 4)
		{
			*(volatile uint32_t*)dst = *(volatile uint32_t*)src;
			item_len = 4;
		}
		else if (len >= 2)
		{
			*(volatile uint16_t*)dst = *(volatile uint16_t*)src;
			item_len = 2;
		}
		else
		{
			*(volatile uint8_t*)dst = *(volatile uint8_t*)src;
			item_len = 1;
		}

		len -= item_len;
		dst += item_len;
		src += item_len;
	}
}

#endif // HL_STM32L1XX

} // namespace hl
