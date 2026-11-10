#include "matrix.hh"

input::MatrixButton MATRIX_BUF[] = {
    // comments are in (row, col) format

    input::MatrixButton(input::Save), // 0, 0
    input::MatrixButton(input::Prev), // 0, 1
    input::MatrixButton(input::Next), // 0, 2
    input::MatrixButton(input::First), // 0, 3
    input::MatrixButton(input::Last), // 0, 4

    input::MatrixButton(input::Start), // 1, 0
    input::MatrixButton(input::None), // 1, 1 // TODO I think save loop is kind of unnecessary
    input::MatrixButton(input::Stop), // 1, 2
    input::MatrixButton(input::None), // 1, 3 (empty on board)
    input::MatrixButton(input::None), // 1, 4 // TODO lock???

    input::MatrixButton(input::SelectLoop, 1), // 2, 0
    input::MatrixButton(input::SelectLoop, 2), // 2, 1
    input::MatrixButton(input::SelectLoop, 3), // 2, 2
    input::MatrixButton(input::SelectLoop, 4), // 2, 3
    input::MatrixButton(input::SelectLoop, 5), // 2, 4
};
