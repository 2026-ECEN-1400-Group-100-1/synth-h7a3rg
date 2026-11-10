#include "matrix.hh"

input::MatrixButton MATRIX_BUF[] = {
    input::MatrixButton(input::Save), // 1, 1
    input::MatrixButton(input::Start), // 1, 2
    input::MatrixButton(input::SelectLoop, 1), // 1, 3

    input::MatrixButton(input::Prev), // 2, 1
    input::MatrixButton(input::None), // 2, 2 // TODO I think save loop is kind of unnecessary
    input::MatrixButton(input::SelectLoop, 2), // 2, 3

    input::MatrixButton(input::Next), // 3, 1
    input::MatrixButton(input::Stop), // 3, 2
    input::MatrixButton(input::SelectLoop, 3), // 3, 3

    input::MatrixButton(input::First), // 4, 1
    input::MatrixButton(input::None), // 4, 2 (empty on board)
    input::MatrixButton(input::SelectLoop, 4), // 4, 3

    input::MatrixButton(input::Last), // 5, 1
    input::MatrixButton(input::None), // 5, 2 TODO lock???
    input::MatrixButton(input::SelectLoop, 5), // 5, 3
};
