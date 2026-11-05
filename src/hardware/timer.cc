#include "timer.hh"

#define MAX_TIMERS 16

namespace {

hardware::Timer* timers[MAX_TIMERS];
uint8_t num_timers = 0;

}

namespace hardware {

Timer::~Timer() {
    // TODO if this ever becomes a problem (probably not)
}

void Timer::handle_timer_interrupt(TIM_HandleTypeDef* htim) {
    for (int i = 0; i < num_timers; ++i) {
        if (timers[i]->timer() == htim) {
            timers[i]->callback();
            return;
        }
    }
}

bool Timer::register_timer() {
    if (num_timers >= MAX_TIMERS)
        return false; // TODO throw/log error

    timer_index = num_timers++;
    timers[timer_index] = this;

    return true;
}

void Timer::start() {
    HAL_TIM_Base_Start_IT(_timer);
}

void Timer::stop() {
    HAL_TIM_Base_Stop_IT(_timer);
}
} // namespace hardware

// the actual interrupt called by the timer on interrupt
// it just defers to the static handler in Timer
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    hardware::Timer::handle_timer_interrupt(htim);
}
