#include "matrix.hh"

namespace input {

void MatrixHandler::update(uint32_t cycles) {
    if (!should_update(cycles))
        return;

    for (uint8_t x = 0; x < _rows; ++x) {
        // set LOW (pullup)
        _matrix.Rows[x].write(GPIO_PIN_RESET);

        for (uint8_t y = 0; y < _cols; ++y) {
            // invert due to pullup
            _matrix_buf[x][y] = !_matrix.Cols[y].read();
        }

        // set back HIGH
        _matrix.Rows[x].write(GPIO_PIN_SET);
    }
}

bool MatrixHandler::get_state(uint8_t x, uint8_t y) const {
    // exit if out of bounds
    if (x >= _rows || y >= _cols)
        return false;

    return _matrix_buf[x][y];
}

bool MatrixHandler::should_update(uint32_t cycles) const {
    return _last_update + _update_threshold < cycles;
}

void MatrixHandler::init_pins() const {
    for (uint8_t x = 0; x < _rows; ++x) {
        _matrix.Rows[x].write(GPIO_PIN_SET);
    }
}

} // namespace input
