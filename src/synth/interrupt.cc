#include "interrupt.hh"

#include "main.h"
#include "defs/keys.h"
#include "defs/notes.h"

namespace synth {

void SynthTimer::callback() {
    _dac_accumulator += 1.0 / _interrupt_frequency;

    double dac_out = 0;
    uint8_t num_keys = 0;

    for (int i = 0; i < NUM_KEYS; ++i) {
        if (key_freq(i) == 0)
            continue;

        ++num_keys;
        dac_out += sin(2 * M_PI * key_freq(i) * _dac_accumulator) + 1;
        break;
    }

    dac_out *= 4095;
    dac_out /= static_cast<double>(num_keys);

    HAL_DAC_SetValue(_dac, _dac_channel, _dac_align,
        static_cast<uint16_t>(dac_out));
}

}
