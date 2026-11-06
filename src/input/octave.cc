#include "octave.hh"

namespace input {

void OctaveHandler::update() {
    bool read_oct_up = _keys.octave_up.read();
    if (read_oct_up != _octave_up_prev) {
        _octave_up_prev = read_oct_up;
        if (read_oct_up)
            ++_curr_octave;
        // TODO debounce
    }

    bool read_oct_down = _keys.octave_down.read();
    if (read_oct_down != _octave_down_prev) {
        _octave_down_prev = read_oct_down;
        if (read_oct_down)
            --_curr_octave;
        // TODO debounce
    }
}

} // namespace input
