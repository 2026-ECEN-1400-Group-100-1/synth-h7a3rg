#include "keys.h"

#include "stm32h7xx_hal.h"
#include "notes.h"

uint8_t key_states[NUM_KEYS];
static uint8_t curr_octave = 4;

typedef struct Key {
    GPIO_TypeDef* Letter;
    uint16_t Pin;
} Key;

const static Key KEYS[NUM_KEYS] = {
    {GPIOC, GPIO_PIN_15},
    {GPIOC, GPIO_PIN_14},
    {GPIOC, GPIO_PIN_13},
    {GPIOB, GPIO_PIN_8},
    {GPIOB, GPIO_PIN_7},
    {GPIOB, GPIO_PIN_6},
    {GPIOB, GPIO_PIN_5},
    {GPIOB, GPIO_PIN_4},
    {GPIOB, GPIO_PIN_3},
    {GPIOD, GPIO_PIN_2},
    {GPIOC, GPIO_PIN_12},
    {GPIOC, GPIO_PIN_11},
};

void update_keys() {
    for (int i = 0; i < NUM_KEYS; ++i) {
        key_states[i] = !HAL_GPIO_ReadPin(KEYS[i].Letter, KEYS[i].Pin);
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
