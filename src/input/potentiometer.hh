#pragma once

#include "handler.hh"
#include "hardware/adc.hh"
#include "utils/event.hh"

namespace input {

enum PotentiometerType {
    None,
};

struct PotentiometerInput {
    PotentiometerInput() = default;
    PotentiometerInput(const ADC_ChannelConfTypeDef& pot_channel, PotentiometerType pot_type)
        : channel(pot_channel), type(pot_type) {}

    ADC_ChannelConfTypeDef channel;
    PotentiometerType type = None;
    uint16_t value = 0;
};

class PotentiometerHandler final : public InputHandler {
public:
    PotentiometerHandler(const hardware::ADC& adc,
        PotentiometerInput* pot_input_buf, uint8_t num_pots)
        : _adc(adc), _pot_buf(pot_input_buf), _num_pots(num_pots) {}

protected:
    void update(uint32_t cycles) override;

private:
    const hardware::ADC& _adc;

    PotentiometerInput* _pot_buf;
    uint8_t _num_pots;
};

class PotentiometerUpdateEvent : public utils::Event<PotentiometerUpdateEvent> {
public:
    PotentiometerUpdateEvent(PotentiometerType pot_type, uint16_t pot_value)
        : Event(*this), type(pot_type), value(pot_value) {}

    PotentiometerType type;
    uint16_t value;
};

} // namespace input
