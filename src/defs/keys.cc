#include "keys.h"

#include "stm32h7xx_hal.h"
#include "notes.h"

#include "hardware/pin.hh"

uint8_t key_states[NUM_KEYS];
uint8_t curr_octave = 4;

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

const static hardware::Pin OCTAVE_CTRL_KEYS[2] = {
    {GPIOB, GPIO_PIN_10},
    {GPIOB, GPIO_PIN_2},
};

void update_keys() {
    for (int i = 0; i < NUM_KEYS; ++i) {
        key_states[i] = !KEYS[i].read();
    }
}

void update_octave_from_keys() {
    static bool oct_up_state = true;
    static bool oct_down_state = true;

    if (OCTAVE_CTRL_KEYS[0].read() != oct_up_state) {
        oct_up_state = !oct_up_state;
        add_octave();
        HAL_Delay(50); // TODO make this not suck
    }

    if (OCTAVE_CTRL_KEYS[1].read() != oct_down_state) {
        oct_down_state = !oct_down_state;
        subtract_octave();
        HAL_Delay(50); // TODO make this not suck
    }
}

uint16_t key_freq(uint8_t key_num) {
    return key_states[key_num] * KEYBOARD_NOTES[curr_octave][key_num];
}

void set_octave(uint8_t octave) {
    curr_octave = octave;
}

void add_octave() {
    curr_octave = (curr_octave + 1) % NUM_OCTAVES;
}

void subtract_octave() {
    curr_octave = (curr_octave - 1) % NUM_OCTAVES;
}
