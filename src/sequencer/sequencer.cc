#include "sequencer.hh"

namespace sequencer {

void Sequencer::callback() {
    _tick += 1;
}

void Sequencer::loop() {
    _tick = 0;

    for (uint8_t i = 0; i < NUM_TRACKS; ++i) {
        _track_currs[i] = _track_heads[i];
    }
}

} // namespace sequencer
