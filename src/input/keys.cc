#include "keys.hh"

namespace input {

void KeyHandler::update(uint32_t cycles) {
    for (uint8_t i = 0; i < NUM_KEYS; ++i) {
        // inverted logic due to pullup resistors
        _key_states[i] = !KEYS[i].read();
    }
}

uint16_t KeyHandler::key_freq(uint8_t key_num) const {
    if (!key_state(key_num))
        return 0;

    return KEYBOARD_NOTES[_octave_ctrl.curr_octave()][key_num];
}

} // namespace input
