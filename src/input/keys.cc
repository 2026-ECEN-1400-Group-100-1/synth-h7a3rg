#include "keys.hh"

namespace input {

void KeyHandler::update(uint32_t cycles) {
    bool changed = false;
    for (uint8_t i = 0; i < NUM_KEYS; ++i) {
        // inverted logic due to pullup resistors
        bool key_state = !KEYS[i].read();
        if (key_state == _key_states[i])
            continue;

        changed = true;
        _key_states[i] = !KEYS[i].read();
    }
    if (!changed) return;

    KeysChangedEvent event(*this);
    event.raise();
}

uint16_t KeyHandler::key_freq(uint8_t key_num) const {
    if (!key_state(key_num))
        return 0;

    return KEYBOARD_NOTES[_octave_ctrl.curr_octave()][key_num];
}

} // namespace input
