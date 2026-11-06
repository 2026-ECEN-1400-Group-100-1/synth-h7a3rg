#pragma once

#include "handler.hh"
#include "defs/keys.hh"

namespace input {

class KeyHandler final : public InputHandler {
public:
    KeyHandler() : _key_states() {}

    void update() override;

    [[nodiscard]]
    bool key_state(uint8_t key_index) const { return _key_states[key_index]; }

private:
    bool _key_states[NUM_KEYS];
};

} // namespace input
