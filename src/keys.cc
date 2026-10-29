#include "keys.h"

#include "stm32h7xx_hal.h"
#include "notes.h"

#define NUM_KEYS 12

uint8_t key_states[NUM_KEYS];

typedef struct Key {
    GPIO_TypeDef* Letter;
    uint16_t Pin;
} Key;

const static Key KEYS[NUM_KEYS] = {
    {GPIOC, GPIO_PIN_15},
    {GPIOC, GPIO_PIN_14},
    {GPIOC, GPIO_PIN_13},
    {GPIOB, GPIO_PIN_8},
};

void update_keys() {
    for (int i = 0; i < NUM_KEYS; ++i) {
        key_states[i] = !HAL_GPIO_ReadPin(KEYS[i].Letter, KEYS[i].Pin);
    }
}

uint16_t key_freq(uint8_t key_num) {
    return key_states[key_num] * KEYBOARD_NOTES[5][key_num]; // TODO octave
}
