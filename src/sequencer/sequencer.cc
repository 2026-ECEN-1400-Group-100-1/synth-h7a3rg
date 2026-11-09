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
    if (event.args().button_pressed.button_type == input::Start) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
    } else if (event.args().button_pressed.button_type == input::Stop) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
    }
}

} // namespace sequencer
