#pragma once

#include "stm32h7xx_hal.h"

namespace hardware {

class ADC {
public:
    explicit ADC(ADC_HandleTypeDef* adc)
        : _handle(adc) {}

    void start() const {
        // TODO calibrate?
        // HAL_ADCEx_Calibration_Start(_handle, ADC_SINGLE_ENDED);
        HAL_ADC_Start(_handle);
    }

    void stop() const {
        HAL_ADC_Stop(_handle);
    }

    [[nodiscard]]
    uint16_t read_channel(ADC_ChannelConfTypeDef& channel) const {
        set_channel(channel);
        return read();
    }

private:
    HAL_StatusTypeDef set_channel(ADC_ChannelConfTypeDef& channel) const {
        return HAL_ADC_ConfigChannel(_handle, &channel);
    }

    [[nodiscard]]
    uint16_t read() const {
        return HAL_ADC_GetValue(_handle);
    }

    ADC_HandleTypeDef* _handle;
};

} // namespace hardware
