#include "keys.h"

#include "stm32h7xx_hal.h"
#include "notes.h"

#include "hardware/pin.hh"

uint8_t key_states[NUM_KEYS];

const static hardware::Pin KEYS[NUM_KEYS] = {
    {GPIOA, GPIO_PIN_8},
    {GPIOA, GPIO_PIN_4},
    {GPIOC, GPIO_PIN_9},
    {GPIOA, GPIO_PIN_5},
    {GPIOC, GPIO_PIN_7},
    {GPIOB, GPIO_PIN_2},
    {GPIOC, GPIO_PIN_6},
    {GPIOB, GPIO_PIN_10},
    {GPIOB, GPIO_PIN_15},
    {GPIOB, GPIO_PIN_12},
    {GPIOB, GPIO_PIN_14},
    {GPIOA, GPIO_PIN_3},
};

void update_keys() {
    for (int i = 0; i < NUM_KEYS; ++i) {
        key_states[i] = !KEYS[i].read();
    }
}

uint16_t key_freq(uint8_t key_num) {
    return key_states[key_num] * KEYBOARD_NOTES[curr_octave][key_num];
}
