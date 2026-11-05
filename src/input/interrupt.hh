#pragma once

#include "hardware/timer.hh"

namespace input {

class InputTimer final : public hardware::Timer {
public:
    explicit InputTimer(TIM_HandleTypeDef* timer) : Timer(timer) {}

protected:
    void callback() override;
};

}
