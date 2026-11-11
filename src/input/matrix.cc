#include "matrix.hh"

namespace input {

void MatrixHandler::update(uint32_t cycles) {
    if (!should_update(cycles))
        return;

    for (uint8_t r = 0; r < _rows; ++r) {
        // set LOW (pullup)
        _matrix.Rows[r].write(GPIO_PIN_RESET);

        for (uint8_t c = 0; c < _cols; ++c) {
            update_button(r, c);
        }

        // set back HIGH
        _matrix.Rows[r].write(GPIO_PIN_SET);
    }
}

bool MatrixHandler::should_update(uint32_t cycles) const {
    return _last_update + _update_threshold < cycles;
}

void MatrixHandler::update_button(uint8_t r, uint8_t c) const {
    bool pin_state = !_matrix.Cols[c].read(); // inverted due to pullup
    MatrixButton& button = buf_at(r, c);

    // return if the state is the same as last time
    if (pin_state == button.state)
        return;

    button.state = pin_state;

    // return if the pin just got toggled off
    if (!pin_state)
        return;

    // raise an update event on this button
    MatrixButtonPressedEvent event(*this, button);
    event.raise();
}

void MatrixHandler::init_pins() const {
    for (uint8_t r = 0; r < _rows; ++r) {
        _matrix.Rows[r].write(GPIO_PIN_SET);
    }
}

} // namespace input
