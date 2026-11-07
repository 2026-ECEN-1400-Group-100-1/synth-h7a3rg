#pragma once

#include "stm32h7xx_hal.h"

namespace hardware {

struct Pin {
    GPIO_TypeDef* Letter;
    uint16_t Number;

    [[nodiscard]]
    bool read() const;

    void write(GPIO_PinState value) const;
    void write(bool value) const;

    void toggle() const;
};

} // namespace hardware
