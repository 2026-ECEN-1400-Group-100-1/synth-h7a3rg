#pragma once

#include "defs/keys.hh"
#include "hardware/pin.hh"
#include "handler.hh"

namespace input {

class OctaveHandler final : public InputHandler {
public:
    OctaveHandler() = default;

    void update() override;

    [[nodiscard]]
    uint8_t curr_octave() const { return _curr_octave; }

private:
    bool _octave_up_prev = false;
    bool _octave_down_prev = false;

    uint8_t _curr_octave = 4;
};

} // namespace input
