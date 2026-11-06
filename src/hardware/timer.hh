#pragma once

#include "stm32h7xx_hal.h"

namespace hardware {

typedef void (*TimerCallback)();

class Timer {
    // Let the HAL timer callback access private functions (handle_timer_interrupt)
    friend void ::HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);

public:
    /// @brief Initializes and registers a timer
    /// @note The timer is not immediately started, start() must be called
    explicit Timer(TIM_HandleTypeDef* timer)
        : _timer(timer) { register_timer(); }

    // No copy or move
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(Timer&&) = delete;

    /// @brief Currently does nothing. Don't let timers leave scope!
    virtual ~Timer();

    /// @brief Returns the TIM_HandleTypeDef* of the timer
    [[nodiscard]]
    TIM_HandleTypeDef* timer() const {
        return _timer;
    }

    /// @brief Starts the timer, meaning its callback will be called on trigger
    void start();

    /// @brief Stops the timer
    void stop();

protected:
    /// @brief The function that will be called when the timer triggers
    virtual void callback() = 0;

private:
    /// @brief Called only by HAL_TIM_PeriodElapsedCallback
    static void handle_timer_interrupt(TIM_HandleTypeDef* timer);

    /// @brief Adds the timer to the timer array so its callback can be called
    /// @note Should only be called by the constructor
    bool register_timer();

    TIM_HandleTypeDef* _timer;

    uint8_t timer_index = 0;
};

} // namespace hardware
