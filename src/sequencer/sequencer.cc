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

void Sequencer::handle_event(utils::Event<input::KeysChangedEvent>& event) {

}

void Sequencer::handle_event(utils::Event<input::MatrixButtonPressedEvent>& event) {
    // giant fucking case switch :(
    switch (event.args().button_pressed.button_type) {
        case input::Start:
            on_start();
            break;
        case input::Stop:
            on_stop();
            break;
        default: return;
    }
}

void Sequencer::on_start() {

}

void Sequencer::on_stop() {

}


} // namespace sequencer
