#pragma once

#include "hardware/timer.hh"

#include "handler.hh"

#define MAX_INPUT_HANDLERS 16

namespace input {

class InputTimer final : public hardware::Timer {
public:
    explicit InputTimer(TIM_HandleTypeDef* timer) : Timer(timer), _handlers() {}

    /// @brief Register an InputHandler
    /// @note `handler` must stay in scope for as long as the InputTimer does,
    /// so it should likely be some sort of global-scoped variable
    bool register_handler(InputHandler* handler);

protected:
    void callback() override;

private:
    InputHandler* _handlers[MAX_INPUT_HANDLERS];
    uint8_t _num_handlers = 0;
};

} // namespace input
