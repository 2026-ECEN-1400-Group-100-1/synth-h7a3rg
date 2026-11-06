#pragma once

#include "hardware/dac.hh"
#include "hardware/timer.hh"

namespace synth {

class SynthTimer final : public hardware::Timer {
public:
    explicit SynthTimer(TIM_HandleTypeDef* timer,
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

}
