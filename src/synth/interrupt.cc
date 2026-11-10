#include "interrupt.hh"

#include "main.h"

namespace synth {

void SynthTimer::callback() {
    _dac_accumulator += 1.0 / _interrupt_frequency;

    if (_dac_accumulator > 1)
        _dac_accumulator -= 1;

    SynthGetFrequenciesEvent event;
    event.raise();

    if (event.frequency_count == 0)
        return;

    double dac_out = 0;

    for (int i = 0; i < event.frequency_count; ++i) {
        dac_out += sin(2 * M_PI * event.frequencies[i] * _dac_accumulator) + 1;
    }

    dac_out *= 2048;
    dac_out /= static_cast<double>(event.frequency_count);

    _dac.write(static_cast<uint16_t>(dac_out));
}

void SynthGetFrequenciesEvent::add_frequency(uint16_t freq) {
    if (frequency_count >= MAX_FREQUENCIES) return;

    // check if it's already in
    for (uint8_t i = 0; i < frequency_count; ++i) {
        if (frequencies[i] == freq) return;
    }

    frequencies[frequency_count++] = freq;
}

} // namespace synth
