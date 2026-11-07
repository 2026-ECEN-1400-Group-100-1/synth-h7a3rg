#pragma once

#include "notes.h"
#include "hardware/pin.hh"

#define NUM_OCTAVES 5
#define DEFAULT_OCTAVE 2
#define NUM_KEYS 12

/// @brief Note definitions for the keyboard
/// @note The x dimension is the octave, the y dimension is the key
const uint16_t KEYBOARD_NOTES[NUM_OCTAVES][NUM_KEYS] = {
    {NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3},
    {NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4},
    {NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5},
    {NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6},
    {NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7},
};

/// @brief The hardware pins for the keys
const hardware::Pin KEYS[NUM_KEYS] = {
    {GPIOA, GPIO_PIN_8},
    {GPIOA, GPIO_PIN_4},
    {GPIOC, GPIO_PIN_9},
    {GPIOA, GPIO_PIN_5},
    {GPIOC, GPIO_PIN_7},
    {GPIOB, GPIO_PIN_2},
    {GPIOC, GPIO_PIN_6},
    {GPIOB, GPIO_PIN_10},
    {GPIOB, GPIO_PIN_15},
    {GPIOB, GPIO_PIN_12},
    {GPIOB, GPIO_PIN_14},
    {GPIOA, GPIO_PIN_3},
};

/// @brief Struct to contain octave up and down hardware pins
struct OctaveKeys {
    const hardware::Pin octave_up;
    const hardware::Pin octave_down;
};

/// @brief The hardware pins for the octave keys
const OctaveKeys OCTAVE_KEYS = {
    {GPIOA, GPIO_PIN_10},
    {GPIOA, GPIO_PIN_9},
};

/// @brief Struct to contain matrix definitions, such as size and pinout
struct Matrix {
    const hardware::Pin* Rows;
    const hardware::Pin* Cols;

    const uint8_t RowCount;
    const uint8_t ColCount;
};

const hardware::Pin MATRIX_ROW_PINS[] = {
    {GPIOA, GPIO_PIN_15},
    {GPIOC, GPIO_PIN_10},
    {GPIOC, GPIO_PIN_11},
};
constexpr uint8_t MATRIX_ROWS = sizeof(MATRIX_ROW_PINS) / sizeof(hardware::Pin);

const hardware::Pin MATRIX_COL_PINS[] = {
    {GPIOD, GPIO_PIN_2},
    {GPIOB, GPIO_PIN_3},
    {GPIOB, GPIO_PIN_4},
    {GPIOB, GPIO_PIN_5},
    {GPIOB, GPIO_PIN_6},
};
constexpr uint8_t MATRIX_COLS = sizeof(MATRIX_COL_PINS) / sizeof(hardware::Pin);

const Matrix MATRIX_KEYS = {
    .Rows = MATRIX_ROW_PINS,
    .Cols = MATRIX_COL_PINS,
    .RowCount = MATRIX_ROWS,
    .ColCount = MATRIX_COLS,
};
