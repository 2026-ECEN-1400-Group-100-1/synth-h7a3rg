#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint8_t key_states[12];

/// @brief Updates the key_states array with the current state of the piano keys
void update_keys();
/// @brief Returns the frequency that a given key should be playing right now
uint16_t key_freq(uint8_t key_num);

#ifdef __cplusplus
}
#endif
