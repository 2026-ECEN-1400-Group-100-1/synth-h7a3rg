#include "pmain.h"

#include <cstdint>

#include "main.h"
#include "stm32h7xx_hal_conf.h" // does weird circular include bullshit if you move this. Thanks STMicro!

#include "sintab.h"

void pmain() {
    // generate sin table
    generate_sintab();

    HAL_TIM_Base_Start_IT(&htim1);
    HAL_DAC_Start(&hdac2, DAC_CHANNEL_1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    static uint16_t dac_val;
    // triangle wave
    dac_val = (dac_val + 1) % 4095;
}
