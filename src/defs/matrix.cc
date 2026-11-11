#include "matrix.hh"

input::MatrixButton MATRIX_BUF[] = {
    // comments are in (row, col) format

    input::MatrixButton(input::SaveLoop), // 0, 0
    input::MatrixButton(input::PrevChord), // 0, 1
    input::MatrixButton(input::NextChord), // 0, 2
    input::MatrixButton(input::FirstChord), // 0, 3
    input::MatrixButton(input::LastChord), // 0, 4

    input::MatrixButton(input::StartLoop), // 1, 0
    input::MatrixButton(input::None), // 1, 1
    input::MatrixButton(input::StopLoop), // 1, 2
    input::MatrixButton(input::None), // 1, 3 (empty on board)
    input::MatrixButton(input::Run), // 1, 4

    input::MatrixButton(input::SelectLoop, 1), // 2, 0
    input::MatrixButton(input::SelectLoop, 2), // 2, 1
    input::MatrixButton(input::SelectLoop, 3), // 2, 2
    input::MatrixButton(input::SelectLoop, 4), // 2, 3
    input::MatrixButton(input::SelectLoop, 5), // 2, 4
};
