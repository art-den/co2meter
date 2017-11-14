#pragma once

void sound_init();

void sound_do_not_play();
void sound_play_warn(unsigned beep_count);
void sound_play_alarm();

void sound_tick_handler();