#pragma once

#include "stm32h7xx_hal.h"

namespace hardware {

struct Pin {
    GPIO_TypeDef* Letter;
    uint16_t Number;

    [[nodiscard]]
    bool read() const;

    void write(GPIO_PinState value);
    void write(bool value);

    void toggle();
};

} // namespace hardware
