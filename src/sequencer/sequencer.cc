#include "sequencer.hh"

namespace sequencer {

void SequenceNode::append_data(const SequenceNode& other) {
    for (uint8_t i = 0; i < other.frequency_count; ++i) {
        // check if it's already in the list
        bool found = false;
        for (uint8_t j = 0; j < frequency_count; ++j) {
            if (frequencies[j] == other.frequencies[i]) {
                found = true;
                break;
            }
        }
        if (found) continue;

        // check list length
        if (frequency_count >= MAX_NOTES_PER_NODE) return;

        // add frequency to the array
        frequencies[frequency_count++] = other.frequencies[i];
    }
}

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
        new_node->next = nullptr;
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
        _tracks[i].curr = _tracks[i].head;
    }
}

void Sequencer::set_last() {
    _beat = _max_beats;
    for (uint8_t i = 0; i < NUM_TRACKS; ++i) {
        // just set all the curr_nodes to tail
        _tracks[i].curr = _tracks[i].tail;
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

void Sequencer::commit_staged() {
    if (_curr_loop == 0) return;

    // get current node
    utils::PoolNode<SequenceNode>*& curr_node = _tracks[_curr_loop-1].curr;
    if (curr_node == nullptr) return;

    // append its data to the staged
    curr_node->data.append_data(_staged_node);

    // reset staged to zero (just leave and ignore junk)
    _staged_node.frequency_count = 0;
}

void Sequencer::handle_event(input::KeysChangedEvent& event) {
    // return if we don't have a selected loop
    if (_curr_loop == 0) return;

    // update staged node state
    for (uint8_t i = 0; i < NUM_KEYS; ++i) {
        uint16_t freq = event.key_handler.key_freq(i);
        if (freq == 0) continue;

        // quit if there isn't space for the note
        if (_staged_node.frequency_count >= MAX_NOTES_PER_NODE) return;

        // check if frequency is already in the list
        bool found = false;
        for (uint8_t j = 0; j < _staged_node.frequency_count; ++j) {
            if (_staged_node.frequencies[j] == freq) {
                found = true;
                break;
            }
        }
        if (found) continue;

        _staged_node.frequencies[_staged_node.frequency_count++] = freq;
    }
}

void Sequencer::handle_event(input::MatrixButtonPressedEvent& event) {
    // giant fucking case switch :(
    switch (event.button_pressed.button_type) {
        case input::Start:
            return on_start();
        case input::Stop:
            return on_stop();
        case input::Save:
            return on_save();
        case input::First:
            return on_first();
        case input::Next:
            return on_next();
        case input::Prev:
            return on_prev();
        case input::Last:
            return on_last();
        case input::SelectLoop: {
            on_select_loop(event.button_pressed.loop_number);
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

void Sequencer::on_save() {
    commit_staged();
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
