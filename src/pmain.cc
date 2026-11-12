#include "pmain.h"

#include "main.h"
#include "stm32h7xx_hal.h"

#include "synth/sintab.h"

#include "hardware/dac.hh"

#include "input/interrupt.hh"
#include "input/keys.hh"
#include "input/matrix.hh"
#include "input/octave.hh"
#include "sequencer/sequencer.hh"
#include "synth/interrupt.hh"

#include "defs/matrix.hh"

static input::InputTimer input_tim(&htim2);
static input::OctaveHandler octave_handle;
static input::KeyHandler key_handle(octave_handle);

static input::MatrixHandler matrix_handle(
    MATRIX_KEYS, MATRIX_BUF, MATRIX_ROWS, MATRIX_COLS);

#define SEQUENCE_NODE_COUNT 2048

// __attribute__(( section(".ram") )) // Put them in RAM (1MB capacity) instead of DTCMRAM (default, 128KB capacity)
static utils::PoolNode<sequencer::SequenceNode> sequence_nodes[SEQUENCE_NODE_COUNT];
static utils::PoolManager seq_pool_man(sequence_nodes, SEQUENCE_NODE_COUNT);
static sequencer::Sequencer seq_tim(&htim3, seq_pool_man);

/// DAC interrupt frequency is 28kHz
#define DAC_INTERRUPT_FREQ 28000

static hardware::DAC dac2(&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R);

static synth::SynthTimer synth_tim(&htim1, dac2, DAC_INTERRUPT_FREQ);

void pmain() {
    // generate sin table
    generate_sintab();

    dac2.start();

    input_tim.register_handler(&octave_handle);
    input_tim.register_handler(&key_handle);
    input_tim.register_handler(&matrix_handle);
    input_tim.start();

    seq_tim.start();

    synth_tim.start();

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);

    for (;;) {
        __NOP();
    }
}
