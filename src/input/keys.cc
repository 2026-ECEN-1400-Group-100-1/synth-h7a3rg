#include "keys.hh"

namespace input {

void KeyHandler::update() {
    for (uint8_t i = 0; i < NUM_KEYS; ++i) {
        _key_states[i] = KEYS[i].read();
    }
}

} // namespace input
