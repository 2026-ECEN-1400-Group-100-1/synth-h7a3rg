#pragma once

#include "hardware/timer.hh"
#include "input/keys.hh"

namespace synth {

class SynthTimer final : public hardware::Timer {
public:
    SynthTimer(TIM_HandleTypeDef* timer,
        DAC_HandleTypeDef* dac,
        uint32_t dac_channel,
        uint32_t dac_align,
        uint32_t interrupt_frequency,
        const input::KeyHandler& key_handler)
        : Timer(timer),
        _dac(dac), _dac_channel(dac_channel), _dac_align(dac_align),
        _interrupt_frequency(interrupt_frequency), _key_handler(key_handler) {}

protected:
    void callback() override;

private:
    DAC_HandleTypeDef* _dac;
    uint32_t _dac_channel;
    uint32_t _dac_align;
    uint32_t _interrupt_frequency;

    const input::KeyHandler& _key_handler;

    double _dac_accumulator = 0;
};

} // namespace synth
