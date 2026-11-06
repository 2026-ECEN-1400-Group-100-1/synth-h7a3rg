#include "pmain.h"

#include "main.h"
#include "stm32h7xx_hal.h"

#include "synth/sintab.h"
#include "defs/keys.h"

#include "hardware/dac.hh"

#include "synth/interrupt.hh"

/// DAC interrupt frequency is 28kHz
#define DAC_INTERRUPT_FREQ 28000

static hardware::DAC dac2(&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R);

static synth::SynthTimer synth_tim(&htim1, dac2, DAC_INTERRUPT_FREQ);

void pmain() {
    // generate sin table
    generate_sintab();

    HAL_DAC_Start(&hdac2, DAC_CHANNEL_1);

    synth_tim.start();

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);

    for (;;) {
        update_keys();
        update_octave_from_keys();
    }
}
