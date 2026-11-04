#pragma once

#include "stm32h7xx_hal.h"

namespace hardware {

typedef void (*TimerCallback)();

class Timer {
public:
    Timer(TIM_HandleTypeDef* timer)
        : _timer(timer) { register_timer(); }

    virtual ~Timer();

    [[nodiscard]]
    TIM_HandleTypeDef* timer() const {
        return _timer;
    }

    virtual void callback() = 0;

    static void handle_timer_interrupt(TIM_HandleTypeDef* timer);

    void start();

private:
    bool register_timer();

    TIM_HandleTypeDef* _timer;

    uint8_t timer_index;
};

} // namespace hardware
