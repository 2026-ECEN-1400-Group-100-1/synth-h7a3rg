#include "pin.hh"

namespace hardware {

bool Pin::read() const {
    return HAL_GPIO_ReadPin(Letter, Number);
}

void Pin::write(GPIO_PinState value) const {
    HAL_GPIO_WritePin(Letter, Number, value);
}

void Pin::write(bool value) const {
    write(static_cast<GPIO_PinState>(value));
}

void Pin::toggle() const {
    HAL_GPIO_TogglePin(Letter, Number);
}

}
