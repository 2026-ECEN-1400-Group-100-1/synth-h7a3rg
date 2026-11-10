#pragma once

#include "handler.hh"
#include "hardware/pin.hh"
#include "defs/keys.hh"
#include "utils/event.hh"

namespace input {

enum MatrixButtonType {
    None = 0,

    Start,
    Stop,

    First,
    Next,
    Prev,
    Last,

    SelectLoop,
};

struct MatrixButton {
    MatrixButton() = default;
    explicit MatrixButton(MatrixButtonType type) : button_type(type) {}

    MatrixButtonType button_type = None;
    bool state = false;
};

struct MatrixLoopSelectButton : MatrixButton {
    MatrixButtonType button_type = SelectLoop;
    uint8_t loop_number = 0;
};

class MatrixHandler final : public InputHandler {
public:
    MatrixHandler(const Matrix& matrix, MatrixButton* matrix_buf, uint8_t rows, uint8_t cols)
        : _matrix(matrix), _matrix_buf(matrix_buf), _rows(rows), _cols(cols)
        { init_pins(); }

    [[nodiscard]]
    bool get_state(uint8_t x, uint8_t y) const;

protected:
    void update(uint32_t cycles) override;

private:
    [[nodiscard]]
    bool should_update(uint32_t cycles) const;

    /// @brief Updates the given button, and raises a MatrixButtonPressedEvent if it's just been pressed
    /// @note Should only be called after row x has been set
    void update_button(uint8_t x, uint8_t y) const;

    /// @brief Sets all the pins to the correct initial value
    void init_pins() const;

    /// @brief Get a reference to the matrix buffer value at (x,y)
    /// @note No bounds check, use get_state for checked
    [[nodiscard]]
    MatrixButton& buf_at(uint8_t x, uint8_t y) const { return _matrix_buf[x * _cols + y]; }

    const Matrix& _matrix;

    ///@note row, col format
    MatrixButton* _matrix_buf;
    uint8_t _rows;
    uint8_t _cols;

    // timeout of 1/6 of a second
    uint16_t _update_threshold = 10;
    uint32_t _last_update = 0;
};

class MatrixButtonPressedEvent : public utils::Event<MatrixButtonPressedEvent> {
public:
    explicit MatrixButtonPressedEvent(const MatrixHandler& event_caller, const MatrixButton& button)
        : Event(*this), matrix_handler(event_caller), button_pressed(button) {}

    const MatrixHandler& matrix_handler;
    const MatrixButton& button_pressed;
};

} // namespace input
