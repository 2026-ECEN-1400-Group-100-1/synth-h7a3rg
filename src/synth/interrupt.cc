#include "interrupt.hh"

#include "main.h"
#include "defs/keys.hh"

namespace synth {

void SynthTimer::callback() {
    _dac_accumulator += 1.0 / _interrupt_frequency;

    double dac_out = 0;
    uint8_t num_keys = 0;

    for (int i = 0; i < NUM_KEYS; ++i) {
        const uint16_t frequency = _key_handler.key_freq(i);
        if (frequency == 0)
            continue;

        ++num_keys;
        dac_out += sin(2 * M_PI * frequency * _dac_accumulator) + 1;
    }

    if (num_keys <= 0)
        return;

    dac_out *= 2048;
    dac_out /= static_cast<double>(num_keys);

    _dac.write(static_cast<uint16_t>(dac_out));
}

} // namespace synth
