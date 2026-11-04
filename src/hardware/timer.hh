#pragma once

#include "stm32h7xx_hal_conf.h"

typedef void (*TimerCallback)();

class Timer {
public:
    Timer(const TimerCallback callback, const TIM_HandleTypeDef* timer)
        : _callback(callback), _timer(timer) {}

    const TIM_HandleTypeDef* timer() const {
        return _timer;
    }

private:
    const TimerCallback _callback;
    const TIM_HandleTypeDef* _timer;
};
