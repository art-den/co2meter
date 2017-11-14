#include "hardware.hpp"
#include "co2unit.hpp"
#include "gui.hpp"
#include "sound.hpp"
#include "config.hpp"

int main()
{
	hardware_init_clock();
	hardware_base_init();

	config_set_defaults();

	gui_init_hardware();

	gui_init();

	gui_show_welcome_screen();

	sound_init();

	co2unit_init_hardware();
	co2unit_init();

	hardware_init_ticks_timer();

	gui_run();

	for (;;) {}
}