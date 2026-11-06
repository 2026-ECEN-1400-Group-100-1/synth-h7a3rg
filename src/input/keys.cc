#include "keys.hh"

namespace input {

void KeyHandler::update() {
    for (uint8_t i = 0; i < NUM_KEYS; ++i) {
        // inverted logic due to pullup resistors
        _key_states[i] = !KEYS[i].read();
    }
}

uint8_t KeyHandler::key_freq(uint8_t key_num) const {
    return KEYBOARD_NOTES[_octave_ctrl.curr_octave()][key_num];
}

} // namespace input
