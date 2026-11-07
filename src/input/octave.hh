#pragma once

#include "defs/keys.hh"
#include "hardware/pin.hh"
#include "handler.hh"

namespace input {

class OctaveHandler final : public InputHandler {
public:
    OctaveHandler() = default;

    [[nodiscard]]
    uint8_t curr_octave() const { return _curr_octave; }

protected:
    void update(uint32_t cycles) override;

private:
    [[nodiscard]]
    bool should_update(uint32_t cycles) const;

    bool _octave_up_prev = false;
    bool _octave_down_prev = false;

    uint8_t _curr_octave = DEFAULT_OCTAVE;

    // timeout of 1/6 of a second
    uint16_t _update_threshold = 10;
    uint32_t _last_update = 0;
};

} // namespace input
