#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint8_t key_states[12];
extern uint8_t curr_octave;

/// @brief Updates the key_states array with the current state of the piano keys
void update_keys();
/// @brief Returns the frequency that a given key should be playing right now
uint16_t key_freq(uint8_t key_num);

/// @brief Handles octave key input
void update_octave_from_keys();

/// @brief Sets the octave to the given value
void set_octave(uint8_t octave);
/// @brief Adds one octave to the current counter
void add_octave();
/// @brief Subtracts one octave to the current counter
void subtract_octave();

#ifdef __cplusplus
}
#endif
