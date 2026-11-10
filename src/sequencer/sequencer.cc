#include "sequencer.hh"

namespace sequencer {

void Sequencer::callback() {
    // exit if we're paused
    if (!_run) return;

    // check if we should move to the next beat
    if (_tick++ < _tempo)
        return;

    // move to next beat with looping
    add_beat_looping();
    _tick = 0;
}

void Sequencer::increment_tracks() {
    for (uint8_t i = 0; i < NUM_TRACKS; ++i) {
        utils::PoolNode<SequenceNode>*& curr_node = _tracks[i].curr;
        if (curr_node == nullptr) continue; // should never happen but segfaults are bad

        curr_node = curr_node->next;
    }
}

void Sequencer::decrement_tracks() {
    for (uint8_t i = 0; i < NUM_TRACKS; ++i) {
        utils::PoolNode<SequenceNode>*& curr_node = _tracks[i].curr;
        if (curr_node == nullptr) continue; // should never happen but segfaults are bad

        curr_node = curr_node->data.prev;
    }
}

void Sequencer::add_trackset() {
    for (uint8_t i = 0; i < NUM_TRACKS; ++i) {
        utils::PoolNode<SequenceNode>*& tail = _tracks[i].tail;
        utils::PoolNode<SequenceNode>* new_node = _pool_man.alloc_one();

        // zero out all the data on the new node
        new_node->data.prev = nullptr;
        for (uint8_t j = 0; j < MAX_NOTES_PER_NODE; ++j)
            new_node->data.frequencies[j] = 0;
        new_node->data.frequency_count = 0;

        if (tail == nullptr) {
            // list is empty, handle heads too
            _tracks[i].head = new_node;
        }

        // link and set
        new_node->data.prev = tail;
        tail->next = new_node;
        tail = new_node;
    }
}

void Sequencer::set_first() {
    _beat = 0;
    for (uint8_t i = 0; i < NUM_TRACKS; ++i) {
        // just set all the curr_nodes to head
        utils::PoolNode<SequenceNode>*& curr_node = _tracks[i].curr;
        curr_node = _tracks->head;
    }
}

void Sequencer::set_last() {
    _beat = _max_beats;
    for (uint8_t i = 0; i < NUM_TRACKS; ++i) {
        utils::PoolNode<SequenceNode>*& curr_node = _tracks[i].curr;
        curr_node = _tracks->tail; // this could probably get fucky but whatever
    }
}

void Sequencer::add_beat_looping() {
    if (_beat++ >= _max_beats) {
        set_first();
    } else {
        increment_tracks();
    }
}

void Sequencer::add_beat_uncapped() {
    // increment max beats if we overflow
    if (_beat++ >= _max_beats) {
        _max_beats = _beat;
        add_trackset();
    }

    // increment only after we've potentially added tracks to increment to
    increment_tracks();
}

void Sequencer::sub_beat_or_cancel() {
    if (_beat == 0)
        return;

    --_beat;
    decrement_tracks();
}

void Sequencer::handle_event(input::KeysChangedEvent& event) {
    // return if we don't have a selected loop
    if (_curr_loop == 0) return;

    // get our node
    utils::PoolNode<SequenceNode>*& curr_node = _tracks[_curr_loop].curr;
    if (curr_node == nullptr) return; // should never happen but segfaults are bad

    // update current loop's state
    for (uint8_t i = 0; i < NUM_KEYS; ++i) {
        uint16_t freq = event.key_handler.key_freq(i);
        if (freq == 0) continue;

        // quit if there isn't space for the note
        if (curr_node->data.frequency_count >= MAX_NOTES_PER_NODE) return;

        // check if frequency is already in the list
        bool found = false;
        for (uint8_t j = 0; j < curr_node->data.frequency_count; ++j) {
            if (curr_node->data.frequencies[j] == freq) {
                found = true;
                break;
            }
        }
        if (found) continue;

        curr_node->data.frequencies[curr_node->data.frequency_count++] = freq;
    }
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
