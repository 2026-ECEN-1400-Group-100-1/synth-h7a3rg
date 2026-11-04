#include "timer.hh"

#define MAX_TIMERS 16

namespace {

hardware::Timer* timers[16];
uint8_t num_timers = 0;

}

namespace hardware {

void Timer::handle_timer_interrupt(TIM_HandleTypeDef* htim) {
    for (int i = 0; i < num_timers; ++i) {
        if (timers[i]->timer() == htim) {
            timers[i]->callback();
            return;
        }
    }
}

void Timer::register_timer() {
    HAL_TIM_Base_Start_IT(_timer);

    if (num_timers >= MAX_TIMERS)
        return; // TODO throw/log error

    timer_index = num_timers++;
    timers[timer_index] = this;
}

} // namespace hardware

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    hardware::Timer::handle_timer_interrupt(htim);
}
