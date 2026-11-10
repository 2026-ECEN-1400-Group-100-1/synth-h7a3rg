#pragma once

#include "hardware/dac.hh"
#include "hardware/timer.hh"
#include "input/keys.hh"

#include "utils/event.hh"

#define MAX_FREQUENCIES 32

namespace synth {

class SynthTimer final : public hardware::Timer {
public:
    SynthTimer(TIM_HandleTypeDef* timer,
        hardware::DAC& dac,
        uint32_t interrupt_frequency,
        const input::KeyHandler& key_handler)
        : Timer(timer),
        _dac(dac),
        _interrupt_frequency(interrupt_frequency), _key_handler(key_handler) {}

protected:
    void callback() override;

private:
    hardware::DAC& _dac;
    uint32_t _interrupt_frequency;

    const input::KeyHandler& _key_handler;

    double _dac_accumulator = 0;
};

class SynthGetFrequenciesEvent final : public utils::Event<SynthGetFrequenciesEvent> {
public:
    SynthGetFrequenciesEvent() : Event(*this), frequencies(), frequency_count(0) {}

    uint16_t frequencies[MAX_FREQUENCIES];
    uint8_t frequency_count;

    void add_frequency(uint16_t freq);
};

} // namespace synth
