#pragma once

#include "hardware/timer.hh"

#include "utils/pool.hh"
#include "defs/keys.hh"
#include "input/keys.hh"
#include "input/matrix.hh"

#include "utils/event.hh"

#define NUM_TRACKS 5
#define MAX_NOTES_PER_NODE 12

namespace sequencer {

// Linked list node, uses next pointer from PoolNode
struct SequenceNode {
    // yucky but whatever
    utils::PoolNode<SequenceNode>* prev;
    uint32_t end_tick;

    bool frequencies[MAX_NOTES_PER_NODE];
    uint8_t frequency_count;
};

class Sequencer final : public hardware::Timer,
    public utils::EventSubscriber<input::KeysChangedEvent>,
    public utils::EventSubscriber<input::MatrixButtonPressedEvent> {
public:
    Sequencer(TIM_HandleTypeDef* tim, utils::PoolManager<SequenceNode>& pool_man)
        : Timer(tim), _pool_man(pool_man), _tracks() {}

    void set_tempo(uint16_t tempo) { _tempo = tempo; }

protected:
    void callback() override;

    void handle_event(input::KeysChangedEvent& event) override;
    void handle_event(input::MatrixButtonPressedEvent& event) override;

private:
    void increment_beat();
    void decrement_beat();

    void set_first();
    void set_last();

    /// @brief Try to add a beat, and loop back to beat 0 if it hits the cap
    void add_beat_looping();
    /// @brief Add a beat, and add to _max_beats if it hits the cap
    void add_beat_uncapped();
    /// @brief Go back a beat, and loop to the end if it hits 0
    void sub_beat_or_cancel();

    void on_start();
    void on_stop();

    void on_first();
    void on_next();
    void on_prev();
    void on_last();

    void on_select_loop(uint8_t loop_num);

    utils::PoolManager<SequenceNode>& _pool_man;

    struct {
        utils::PoolNode<SequenceNode>* head;
        utils::PoolNode<SequenceNode>* tail;
        utils::PoolNode<SequenceNode>* curr;
    } _tracks[NUM_TRACKS];

    uint8_t _curr_loop = 0;

    uint32_t _tick = 0;

    uint16_t _beat = 0;
    uint16_t _max_beats = 0;

    bool _run = false;
    uint16_t _tempo = 60; // TODO set this
    /// @note beats per measure, we don't care about lower part
    uint8_t _time_signature = 4;
};

} // namespace sequencer
