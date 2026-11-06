#include "interrupt.hh"

namespace input {

void InputTimer::callback() {
    for (uint8_t i = 0; i < _num_handlers; ++i) {
        _handlers[i]->update(_cycle);
    }
    ++_cycle;
}

bool InputTimer::register_handler(InputHandler *handler) {
    if (_num_handlers >= MAX_INPUT_HANDLERS)
        return false;

    _handlers[_num_handlers++] = handler;
    return true;
}

} // namespace input
