#pragma once

#include "stm32h7xx_hal.h"

namespace hardware {

class DAC {
public:
    DAC(DAC_HandleTypeDef* handle, uint32_t channel, uint32_t align)
        : _handle(handle), _channel(channel), _align(align) {}

    void start() const {
        HAL_DAC_Start(_handle, _channel);
    }

    void stop() const {
        HAL_DAC_Stop(_handle, _channel);
    }

    uint32_t read() const {
        return HAL_DAC_GetValue(_handle, _channel);
    }

    HAL_StatusTypeDef write(uint32_t val) {
        return HAL_DAC_SetValue(_handle, _channel, _align, val);
    }

private:
    DAC_HandleTypeDef* _handle;
    uint32_t _channel;
    uint32_t _align;
};

} // namespace hardware