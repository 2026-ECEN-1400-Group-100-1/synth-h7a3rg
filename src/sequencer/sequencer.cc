#include "sequencer.hh"

namespace sequencer {

void Sequencer::callback() {
    // exit if we're paused
    if (!_run) return;

    // check if we should move to the next beat
    if (_tick++ < _tempo)
        return;

    // move to next beat
    add_beat_looping();
    _tick = 0;
}

void Sequencer::increment_beat() {
    for (uint8_t i = 0; i < NUM_TRACKS; ++i) {
        utils::PoolNode<SequenceNode>*& curr_node = _tracks[i].curr;
        if (curr_node == nullptr) continue;

        curr_node = curr_node->next;
    }
}

void Sequencer::decrement_beat() {
    for (uint8_t i = 0; i < NUM_TRACKS; ++i) {
        utils::PoolNode<SequenceNode>*& curr_node = _tracks[i].curr;
        if (curr_node == nullptr) continue; // TODO this doesn't go back right if we step off the end of the track

        curr_node = curr_node->data.prev;
    }
}

void Sequencer::set_first() {
    _tick = 0;
    _beat = 0;
    // TODO not fucking copy paste this a million times
    for (uint8_t i = 0; i < NUM_TRACKS; ++i) {
        utils::PoolNode<SequenceNode>*& curr_node = _tracks[i].curr;
        curr_node = _tracks->head;
    }
}

void Sequencer::set_last() {
    _tick = 0;
    _beat = _max_beats;
    for (uint8_t i = 0; i < NUM_TRACKS; ++i) {
        utils::PoolNode<SequenceNode>*& curr_node = _tracks[i].curr;
        curr_node = _tracks->tail; // TODO this gets fucky (like decrement_beat)
    }
}

void Sequencer::add_beat_looping() {
    if (_beat >= _max_beats) {
        set_first();
        return;
    }
    increment_beat();
}

void Sequencer::add_beat_uncapped() {
    _beat++;

    // increment max beats if we just overflowed
    if (_beat > _max_beats)
        _max_beats = _beat;
}

void Sequencer::sub_beat_or_cancel() {
    // cancel if we're at zero
    if (_beat == 0) {
        return;
    }
    decrement_beat();
}

void Sequencer::handle_event(input::KeysChangedEvent& event) {

}

void Sequencer::handle_event(input::MatrixButtonPressedEvent& event) {
    // giant fucking case switch :(
    switch (event.button_pressed.button_type) {
        case input::Start:
            return on_start();
        case input::Stop:
            return on_stop();
        case input::First:
            return on_first();
        case input::Next:
            return on_next();
        case input::Prev:
            return on_prev();
        case input::Last:
            return on_last();
        case input::SelectLoop: {
            // this is kind of yucky but I don't care enough to fix it
            const input::MatrixLoopSelectButton& button =
                static_cast<const input::MatrixLoopSelectButton&>(event.button_pressed);
            on_select_loop(button.loop_number);
            return;
        }
        default: return; // so clang-tidy will shut up
    }
}

void Sequencer::on_start() {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
    _run = true;
}

void Sequencer::on_stop() {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
    _run = false;
}

void Sequencer::on_first() {
    set_first();
}

void Sequencer::on_last() {
    set_last();
}

void Sequencer::on_next() {
    add_beat_uncapped();
}

void Sequencer::on_prev() {
    sub_beat_or_cancel();
}

void Sequencer::on_select_loop(uint8_t loop_num) {
    _curr_loop = loop_num;
}

} // namespace sequencer
