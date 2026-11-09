#include "matrix.hh"

namespace input {

void MatrixHandler::update(uint32_t cycles) {
    if (!should_update(cycles))
        return;

    for (uint8_t y = 0; y < _rows; ++y) {
        // set LOW (pullup)
        _matrix.Rows[y].write(GPIO_PIN_RESET);

        for (uint8_t x = 0; x < _cols; ++x) {
            update_button(x, y);
        }

        // set back HIGH
        _matrix.Rows[y].write(GPIO_PIN_SET);
    }
}

bool MatrixHandler::get_state(uint8_t x, uint8_t y) const {
    // exit if out of bounds
    if (x >= _rows || y >= _cols)
        return false;

    return buf_at(x, y).state;
}

bool MatrixHandler::should_update(uint32_t cycles) const {
    return _last_update + _update_threshold < cycles;
}

void MatrixHandler::update_button(uint8_t x, uint8_t y) const {
    bool pin_state = !_matrix.Cols[x].read(); // inverted due to pullup
    MatrixButton& button = buf_at(x, y);

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
    for (uint8_t x = 0; x < _rows; ++x) {
        _matrix.Rows[x].write(GPIO_PIN_SET);
    }
}

} // namespace input
