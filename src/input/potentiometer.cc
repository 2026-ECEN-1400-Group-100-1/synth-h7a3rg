#include "potentiometer.hh"

namespace input {

void PotentiometerHandler::update(uint32_t cycles) {
    for (uint8_t i = 0; i < _num_pots; ++i) {
        PotentiometerInput& potentiometer = _pot_buf[i];
        potentiometer.value = _adc.read_channel(potentiometer.channel);
    }
}

} // namespace input
