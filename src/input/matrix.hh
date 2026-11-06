#pragma once

#include "handler.hh"
#include "hardware/pin.hh"
#include "defs/keys.hh"

namespace input {

class MatrixHandler final : public InputHandler {
public:
    MatrixHandler(const Matrix& matrix, bool** matrix_buf, uint8_t rows, uint8_t cols)
        : _matrix(matrix), _matrix_buf(matrix_buf), _rows(rows), _cols(cols)
        { init_pins(); }

    [[nodiscard]]
    bool get_state(uint8_t x, uint8_t y) const;

protected:
    void update(uint32_t cycles) override;

private:
    [[nodiscard]]
    bool should_update(uint32_t cycles) const;

    /// @brief Sets all the pins to the correct initial value
    void init_pins() const;

    const Matrix& _matrix;

    bool** _matrix_buf;
    uint8_t _rows;
    uint8_t _cols;

    // timeout of 1/6 of a second
    uint16_t _update_threshold = 10;
    uint32_t _last_update = 0;
};

} // namespace input
