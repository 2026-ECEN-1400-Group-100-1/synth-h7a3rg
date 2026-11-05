#include "pmain.h"

#include <cstdint>

#include "main.h"
#include "stm32h7xx_hal.h"

#include "synth/sintab.h"
#include "defs/keys.h"

#include "input/interrupt.hh"

#include "synth/interrupt.hh"

static input::InputTimer input_tim(&htim2);

/// DAC interrupt frequency is 28kHz
#define DAC_INTERRUPT_FREQ 28000

static synth::SynthTimer synth_tim(&htim1,
    &hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R,
    DAC_INTERRUPT_FREQ);

void pmain() {
    // generate sin table
    generate_sintab();

    HAL_DAC_Start(&hdac2, DAC_CHANNEL_1);

    synth_tim.start();
    input_tim.start();

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);

    for (;;) {
        update_keys();
        update_octave_from_keys();
    }
}
