#include "pmain.h"

#include <cstdint>

#include "main.h"
#include "stm32h7xx_hal.h"

#include "sintab.h"
#include "defs/keys.h"
#include "defs/notes.h"

/// DAC interrupt frequency is 28kHz
#define DAC_INTERRUPT_FREQ 28000

void pmain() {
    // generate sin table
    generate_sintab();

    HAL_TIM_Base_Start_IT(&htim1);
    HAL_DAC_Start(&hdac2, DAC_CHANNEL_1);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);

    for (;;) {
        update_keys();
        update_octave_from_keys();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    static double dac_accumulator;
    if (htim->Instance == htim1.Instance) {
        dac_accumulator += 1.0 / DAC_INTERRUPT_FREQ;

        double dac_out = 0;
        uint8_t num_keys = 0;

        for (int i = 0; i < NUM_KEYS; ++i) {
            if (key_freq(i) == 0)
                continue;

            ++num_keys;
            dac_out += sin(2 * M_PI * key_freq(i) * dac_accumulator) + 1;
            break;
        }

        dac_out *= 4095;
        dac_out /= (double)num_keys;

        HAL_DAC_SetValue(&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint16_t)dac_out);
    }
}
