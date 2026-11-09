#pragma once

#include "hardware/timer.hh"

#include "utils/pool.hh"
#include "defs/keys.hh"
#include "input/keys.hh"
#include "input/matrix.hh"

#include "utils/event.hh"

#define NUM_TRACKS 5

namespace sequencer {

// Linked list node, uses next pointer from PoolNode
struct SequenceNode {
    bool key_states[NUM_KEYS];

    uint32_t start_tick;
    uint16_t length;
};

class Sequencer final : public hardware::Timer,
    public utils::EventSubscriber<input::KeysChangedEvent>,
    public utils::EventSubscriber<input::MatrixButtonPressedEvent> {
public:
    Sequencer(TIM_HandleTypeDef* tim, utils::PoolManager<SequenceNode>& pool_man)
        : Timer(tim), _pool_man(pool_man), _track_heads(), _track_tails(), _track_currs() {}

protected:
    void callback() override;

    void handle_event(input::KeysChangedEvent& event) override;
    void handle_event(input::MatrixButtonPressedEvent& event) override;

private:
    void loop();

    void on_start();
    void on_stop();

    utils::PoolManager<SequenceNode>& _pool_man;

    utils::PoolNode<SequenceNode>* _track_heads[NUM_TRACKS];
    utils::PoolNode<SequenceNode>* _track_tails[NUM_TRACKS];

    utils::PoolNode<SequenceNode>* _track_currs[NUM_TRACKS];

    volatile uint32_t _tick = 0;
};

} // namespace sequencer
