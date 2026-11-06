#pragma once

#include "handler.hh"
#include "octave.hh"
#include "defs/keys.hh"

namespace input {

class KeyHandler final : public InputHandler {
public:
    explicit KeyHandler(const OctaveHandler& octave_ctrl)
        : _key_states(), _octave_ctrl(octave_ctrl) {}

    [[nodiscard]]
    bool key_state(uint8_t key_index) const { return _key_states[key_index]; }

    [[nodiscard]]
    uint8_t key_freq(uint8_t key_num) const;

protected:
    void update() override;

private:
    bool _key_states[NUM_KEYS];

    const OctaveHandler& _octave_ctrl;
};

} // namespace input
