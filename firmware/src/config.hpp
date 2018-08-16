#pragma once

#include <stdint.h>

struct Config
{
	uint16_t ok_value;
	uint16_t warn_value;
	uint16_t alarm_value;
	uint16_t request_period;
};

extern volatile Config config;

void config_set_defaults();