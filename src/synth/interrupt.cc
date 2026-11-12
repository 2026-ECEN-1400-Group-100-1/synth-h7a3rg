#include "interrupt.hh"

#include "main.h"

#include "sintab.h"

namespace synth {

void SynthTimer::callback() {
    _dac_accumulator += 1.0 / _interrupt_frequency;

    // prevent floating point precision loss shenanegans
    // and prevent uint16_t sin_val from overflowing
    if (_dac_accumulator > 1)
        _dac_accumulator -= 1;

    SynthGetFrequenciesEvent event;
    event.raise();

    if (event.frequency_count == 0)
        return;

    uint16_t dac_out = 0;

    // get the wave at the current time for all of our frequencies
    for (int i = 0; i < event.frequency_count; ++i) {
        // NOLINTBEGIN - ignore narrowing conversion warning, we're doing it on purpose
        const uint16_t sin_val =
            static_cast<double>(event.frequencies[i]) * _dac_accumulator;
        // NOLINTEND

        // range of sintab: 0 - 4095
        // domain: 0-4095 (done modulo)
        dac_out += sintab(sin_val);
    }

    // chop to fit in 12-bit DAC
    dac_out /= event.frequency_count * 4096;

    _dac.write(dac_out);
}

bool SynthGetFrequenciesEvent::add_frequency(uint16_t freq) {
    if (frequency_count >= MAX_FREQUENCIES) return false;

    // check if it's already in
    for (uint8_t i = 0; i < frequency_count; ++i) {
        // true feels weird here but it's to check if there's space, not if freq was added
        if (frequencies[i] == freq) return true;
    }

    frequencies[frequency_count++] = freq;
    return true;
}

} // namespace synth
