#pragma once

#include "stm32_lib/hl_gpio.hpp"
#include "stm32_lib/hl_spi.hpp"
#include "stm32_lib/hl_usart.hpp"
#include "stm32_lib/hl_tim.hpp"
#include "muil_display_ILI9341.hpp"

using namespace hl;

constexpr unsigned SysFreq = 48000000;

typedef PC13 WhiteLed;

// Периф для связи с датчиком CO2
typedef Usart<2> CO2Usart;
typedef PA3 CO2UsartRxPin;
typedef PA2 CO2UsartTxPin;

// Звук
typedef Timer4 SoundTimer;
constexpr unsigned SoundTimerChan = 1;
typedef PB6 SoundPin;

// LCD-дисплей
typedef SPI<1> LCD_SPI;
typedef PA0 DisplayCSPin;
typedef PA4 DisplayDCPin;
typedef PA1 DisplayResetPin;
typedef PA5 DisplaySCKPin;
typedef PA6 DisplayMISOPin;
typedef PA7 DisplayMOSIPin;
typedef muil::ILI9341DisplaySPIConnector<LCD_SPI, DisplayCSPin, DisplayResetPin, DisplayDCPin> DisplayConnector;
typedef muil::ILI9341Display<DisplayConnector> DisplayDriver;

// LED-подсветка дисплея
typedef PB0 DisplLedPin;
typedef Timer3 DisplLedTimer;
constexpr unsigned DisplLedTimerChan = 3;

// Таймер запроса данных с датчика
typedef Timer2 TicksTimer;

void hardware_init_clock();
void hardware_base_init();
void hardware_init_ticks_timer();

