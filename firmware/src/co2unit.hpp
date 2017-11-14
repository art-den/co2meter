#pragma once

void co2unit_init_hardware();
void co2unit_init();

void co2unit_start_measure();

unsigned co2unit_get_result();
unsigned co2unit_get_measures_count();