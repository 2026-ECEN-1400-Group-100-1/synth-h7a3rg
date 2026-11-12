#pragma once

#include "stm32h7xx_hal.h"

namespace hardware {

class ADC {
public:
    explicit ADC(ADC_HandleTypeDef* adc)
        : _handle(adc) {}

    HAL_StatusTypeDef start() const {
        // TODO calibrate?
        return HAL_ADC_Start(_handle);
    }

    HAL_StatusTypeDef stop() const {
        return HAL_ADC_Stop(_handle);
    }

    HAL_StatusTypeDef set_channel(ADC_ChannelConfTypeDef* channel) const {
        return HAL_ADC_ConfigChannel(_handle, channel);
    }

    [[nodiscard]]
    uint16_t read() const {
        return HAL_ADC_GetValue(_handle);
    }

private:
    ADC_HandleTypeDef* _handle;
};

class AdcChannel {
public:
    AdcChannel(const ADC& adc, ADC_ChannelConfTypeDef* channel)
        : _adc(adc), _channel(channel) {}

    [[nodiscard]]
    uint16_t read() const {
        _adc.set_channel(_channel);
        return _adc.read();
    }

private:
    const ADC& _adc;
    ADC_ChannelConfTypeDef* _channel;
};

} // namespace hardware
