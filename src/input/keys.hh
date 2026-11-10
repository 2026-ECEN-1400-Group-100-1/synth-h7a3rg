#pragma once

#include "handler.hh"
#include "octave.hh"
#include "defs/keys.hh"

#include "utils/event.hh"

// forward declare because of some stupid bullshit probably
namespace synth {
class SynthGetFrequenciesEvent;
} // namespace synth

namespace input {

class KeyHandler final : public InputHandler, public utils::EventSubscriber<synth::SynthGetFrequenciesEvent> {
public:
    explicit KeyHandler(const OctaveHandler& octave_ctrl)
        : _key_states(), _octave_ctrl(octave_ctrl) {}

    /// @brief Gets the current state (up or down) of key_index
    [[nodiscard]]
    bool key_state(uint8_t key_index) const { return _key_states[key_index]; }

    /// @brief Gets the frequency that the note should be playing (0 if none)
    [[nodiscard]]
    uint16_t key_freq(uint8_t key_num) const;

protected:
    void update(uint32_t cycles) override;

    void handle_event(synth::SynthGetFrequenciesEvent& event) override;

private:
    bool _key_states[NUM_KEYS];

    const OctaveHandler& _octave_ctrl;
};

class KeysChangedEvent final : public utils::Event<KeysChangedEvent> {
public:
    explicit KeysChangedEvent(const KeyHandler& event_caller) : Event(*this), key_handler(event_caller) {}

    const KeyHandler& key_handler;
};

} // namespace input
