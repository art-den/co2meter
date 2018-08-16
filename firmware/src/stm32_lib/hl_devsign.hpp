#pragma once

#include <stdint.h>
#include "hl_device.hpp"

namespace hl {

inline uint32_t get_sign1()
{
	return HL_UI32REG(UID_BASE);
}

inline uint32_t get_sign2()
{
	return HL_UI32REG(UID_BASE+4);
}

inline uint32_t get_sign3()
{
	return HL_UI32REG(UID_BASE+8);
}

} // namespace hl

