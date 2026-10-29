#include "pmain.h"

#include <cstdint>

#include "main.h"
#include "stm32h7xx_hal_conf.h" // does weird circular include bullshit if you move this. Thanks STMicro!

#include "sintab.h"

/// DAC interrupt frequency is 28kHz
#define DAC_INTERRUPT_FREQ 28000

int freq = 440;

void pmain() {
    // generate sin table
    generate_sintab();

    HAL_TIM_Base_Start_IT(&htim1);
    HAL_DAC_Start(&hdac2, DAC_CHANNEL_1);

    for (;;) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
        HAL_Delay(1000);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    static uint16_t dac_accumulator;
    if (htim->Instance == htim1.Instance) {
        // triangle wave
        dac_accumulator = (dac_accumulator + (freq / 7)) % 4000; // bullshit hack
        HAL_DAC_SetValue(&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_accumulator);
    }
}
