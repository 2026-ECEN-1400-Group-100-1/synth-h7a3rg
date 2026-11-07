#include "octave.hh"

namespace input {

void OctaveHandler::update(uint32_t cycles) {
    if (!should_update(cycles))
        return;

    bool read_oct_up = OCTAVE_KEYS.octave_up.read();
    if (read_oct_up != _octave_up_prev) {
        _octave_up_prev = read_oct_up;
        if (read_oct_up)
            ++_curr_octave;
        _last_update = cycles;
    }

    bool read_oct_down = OCTAVE_KEYS.octave_down.read();
    if (read_oct_down != _octave_down_prev) {
        _octave_down_prev = read_oct_down;
        if (read_oct_down)
            --_curr_octave;
        _last_update = cycles;
    }

    // prevent octave overruns
    _curr_octave %= NUM_OCTAVES;
}

bool OctaveHandler::should_update(uint32_t cycles) const {
    return _last_update + _update_threshold < cycles;
}

} // namespace input
