#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint8_t key_states[12];

void update_keys();
uint16_t key_freq(uint8_t key_num);

void set_octave(uint8_t octave);
void add_octave();
void subtract_octave();

#ifdef __cplusplus
}
#endif
