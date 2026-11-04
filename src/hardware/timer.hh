#pragma once

#include "stm32h7xx_hal.h"

namespace hardware {

typedef void (*TimerCallback)();

class Timer {
public:
    Timer(const TimerCallback callback, TIM_HandleTypeDef* timer)
        : _callback(callback), _timer(timer) { register_timer(); }

    [[nodiscard]]
    TIM_HandleTypeDef* timer() const {
        return _timer;
    }

    void callback() const {
        _callback();
    }

    static void handle_timer_interrupt(TIM_HandleTypeDef* timer);

private:
    void register_timer();

    const TimerCallback _callback;
    TIM_HandleTypeDef* _timer;

    uint8_t timer_index;
};

} // namespace hardware
