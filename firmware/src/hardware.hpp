#pragma once

#include "stm32_lib/hl_gpio.hpp"
#include "stm32_lib/hl_usart.hpp"
#include "stm32_lib/hl_tim.hpp"

using namespace hl;

constexpr unsigned SysClockFreq = 96000000;

typedef PC13 WhiteLed;

// Периф для связи с датчиком CO2
typedef Usart<2> CO2Usart;
typedef PA3 CO2UsartRxPin;
typedef PA2 CO2UsartTxPin;

typedef PA0 DebugPin;

// Таймер запроса данных с датчика
typedef Timer4 TicksTimer;

void hardware_init_clock();
void hardware_base_init();
void hardware_init_ticks_timer();

