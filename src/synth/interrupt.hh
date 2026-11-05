#pragma once

#include "hardware/timer.hh"

namespace synth {

class SynthTimer : public hardware::Timer {
public:
    explicit SynthTimer(TIM_HandleTypeDef* timer,
        DAC_HandleTypeDef* dac,
        uint32_t dac_channel,
        uint32_t dac_align,
        uint32_t interrupt_frequency)
        : Timer(timer),
        _dac(dac), _dac_channel(dac_channel), _dac_align(dac_align),
        _interrupt_frequency(interrupt_frequency) {}

    void callback() override;
private:
    DAC_HandleTypeDef* _dac;
    uint32_t _dac_channel;
    uint32_t _dac_align;
    uint32_t _interrupt_frequency;

    double _dac_accumulator = 0;
};

}