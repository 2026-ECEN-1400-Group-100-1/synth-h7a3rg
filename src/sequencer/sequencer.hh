#pragma once

#include "hardware/timer.hh"

#include "utils/pool.hh"
#include "defs/keys.hh"
#include "input/keys.hh"
#include "input/matrix.hh"
#include "synth/interrupt.hh"

#include "utils/event.hh"

#define NUM_TRACKS 5
#define MAX_NOTES_PER_NODE 12

namespace sequencer {

// Linked list node, uses next pointer from PoolNode
struct SequenceNode {
    // yucky but whatever
    utils::PoolNode<SequenceNode>* prev;

    uint16_t frequencies[MAX_NOTES_PER_NODE];
    uint8_t frequency_count;

    void append_data(const SequenceNode& other);
    void add_to_synth_event(synth::SynthGetFrequenciesEvent& event) const;

    void clear() { frequency_count = 0; }
};

class Sequencer final : public hardware::Timer,
    public utils::EventSubscriber<input::KeysChangedEvent>,
    public utils::EventSubscriber<input::MatrixButtonPressedEvent>,
    public utils::EventSubscriber<synth::SynthGetFrequenciesEvent> {
public:
    Sequencer(TIM_HandleTypeDef* tim, utils::PoolManager<SequenceNode>& pool_man)
        : Timer(tim), _pool_man(pool_man), _tracks(), _staged_node() {

        // add an initial trackset at pos 0, and set current nodes to it
        add_trackset();
        set_first();
    }

    void set_tempo(uint16_t tempo) { _tempo = tempo; }

protected:
    void callback() override;

    void handle_event(input::KeysChangedEvent& event) override;
    void handle_event(input::MatrixButtonPressedEvent& event) override;
    void handle_event(synth::SynthGetFrequenciesEvent& event) override;

private:
    /// @brief Move the curr* for every track up by one
    void increment_tracks();
    /// @brief Move the curr* for every track down by one
    void decrement_tracks();

    /// @brief Adds an empty set of nodes to every track
    void add_trackset();

    void set_first();
    void set_last();

    void set_running(bool run);
    void set_track(uint8_t track);

    /// @brief Try to add a beat, and loop back to beat 0 if it hits the cap
    void add_beat_looping();
    /// @brief Add a beat, and add to _max_beats if it hits the cap
    void add_beat_uncapped();
    /// @brief Go back a beat, unless it's already on the first beat
    void sub_beat_or_cancel();

    /// @brief Commit the staged node to the current track
    void commit_staged();

    void on_start();
    void on_stop();

    void on_save();

    void on_first();
    void on_next();
    void on_prev();
    void on_last();

    void on_run();

    void on_select_loop(uint8_t loop_num);

    utils::PoolManager<SequenceNode>& _pool_man;

    struct {
        utils::PoolNode<SequenceNode>* head = nullptr;
        utils::PoolNode<SequenceNode>* tail = nullptr;
        utils::PoolNode<SequenceNode>* curr = nullptr;

        bool running = false;
    } _tracks[NUM_TRACKS];

    SequenceNode _staged_node;

    uint8_t _curr_loop = 0;

    uint32_t _tick = 0;

    uint16_t _beat = 0;
    uint16_t _max_beats = 0;

    bool _run = false;
    uint16_t _tempo = 28; // TODO set this
    uint8_t _time_signature = 4;
};

} // namespace sequencer
