#include "config.hpp"

volatile Config config;

void config_set_defaults()
{
	config.ok_value = 800;
	config.warn_value = 1000;
	config.alarm_value = 1400;
	config.request_period = 30;
}