#include "pmain.h"

#include <cstdint>

#include "main.h"
#include "stm32h7xx_hal.h"

#include "sintab.h"
#include "keys.h"
#include "notes.h"

/// DAC interrupt frequency is 28kHz
#define DAC_INTERRUPT_FREQ 28000

int freq = 440 * 2;

void pmain() {
    // generate sin table
    generate_sintab();

    HAL_TIM_Base_Start_IT(&htim1);
    HAL_DAC_Start(&hdac2, DAC_CHANNEL_1);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);

    for (;;) {
        update_keys();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    static uint16_t dac_accumulator;
    if (htim->Instance == htim1.Instance) {
        // triangle wave
        for (int i = 0; i < NUM_KEYS; ++i) {
            if (key_freq(i) == 0)
                continue;
            dac_accumulator = (dac_accumulator + (key_freq(i) / 7)) % 4000; // bullshit hack
            break;
        }

        HAL_DAC_SetValue(&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_accumulator);
    }
}
