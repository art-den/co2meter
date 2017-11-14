#include "config.hpp"

volatile Config config;

void config_set_defaults()
{
	config.warn_value = 1000;
	config.alarm_value = 1500;
	config.request_period = 30;
}