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

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);

    for (;;) {
        update_keys();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    static double dac_accumulator;
    static uint16_t dac_accum_last_rnd;
    if (htim->Instance == htim1.Instance) {
        dac_accum_last_rnd = (uint16_t)dac_accumulator;
        dac_accumulator += 1.0 / DAC_INTERRUPT_FREQ;

        if ((uint16_t)dac_accumulator != dac_accum_last_rnd)
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);

        double dac_out = 0;
        uint8_t num_keys = 0;

        for (int i = 0; i < NUM_KEYS; ++i) {
            if (key_freq(i) == 0)
                continue;

            ++num_keys;
            dac_out += sin(2 * M_PI * key_freq(i) * dac_accumulator) + 1;
        }

        dac_out *= 4095;
        dac_out /= (double)num_keys;

        HAL_DAC_SetValue(&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint16_t)dac_out);
    }
}
