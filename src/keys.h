#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint8_t key_states[12];

void update_keys();
uint16_t key_freq(uint8_t key_num);

#ifdef __cplusplus
}
#endif
