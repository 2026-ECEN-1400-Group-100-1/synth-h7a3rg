#pragma once

#include "hardware/dac.hh"
#include "hardware/timer.hh"

#include "utils/event.hh"

#define MAX_FREQUENCIES 12

namespace synth {

class SynthTimer final : public hardware::Timer {
public:
    SynthTimer(TIM_HandleTypeDef* timer,
        hardware::DAC& dac,
        uint32_t interrupt_frequency)
        : Timer(timer),
        _dac(dac),
        _interrupt_frequency(interrupt_frequency) {}

protected:
    void callback() override;

private:
    hardware::DAC& _dac;
    uint32_t _interrupt_frequency;

    double _dac_accumulator = 0;
};

class SynthGetFrequenciesEvent final : public utils::Event<SynthGetFrequenciesEvent> {
public:
    // NOLINTNEXTLINE - we don't initialize frequencies[] for optimization reasons
    SynthGetFrequenciesEvent() : Event(*this), frequency_count(0) {}

    uint16_t frequencies[MAX_FREQUENCIES];
    uint8_t frequency_count;

    bool add_frequency(uint16_t freq);
};

} // namespace synth
