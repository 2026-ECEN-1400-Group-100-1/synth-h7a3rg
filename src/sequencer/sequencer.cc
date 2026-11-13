#include "sequencer.hh"

#include "defs/lights.hh"

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

void SequenceNode::add_to_synth_event(synth::SynthGetFrequenciesEvent& event) const {
    for (uint8_t i = 0; i < frequency_count; ++i) {
        if (!event.add_frequency(frequencies[i])) return;
    }
}

void Sequencer::callback() {
    // exit if we're paused
    if (!_run) return;

    // check if we should move to the next beat
    if (_tick++ < _tempo * _time_signature)
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
        if (new_node == nullptr) return; // won't practically happen (>16k nodes in pool)

        // zero out all the data on the new node
        new_node->next = nullptr;
        new_node->data.prev = nullptr;
        // can just leave junk in frequencies
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

void Sequencer::set_running(bool run) {
    if (_run == run) return;

    // TODO make the status light not suck
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, run ? GPIO_PIN_SET : GPIO_PIN_RESET);
    _staged_node.clear();
    _run = run;
}

void Sequencer::set_loop(uint8_t track) {
    if (_curr_loop != 0) {
        LOOP_LED_PINS[track-1].write(false);
    }

    _curr_loop = track;
    if (_curr_loop == 0) return;
    LOOP_LED_PINS[track-1].write(true);
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
    // return if we don't have a loop
    if (_curr_loop == 0) return;

    // get current node
    utils::PoolNode<SequenceNode>*& curr_node = _tracks[_curr_loop-1].curr;
    if (curr_node == nullptr) return;

    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);

    // append its data to the staged
    curr_node->data.append_data(_staged_node);
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
        case input::StartLoop:
            return on_start();
        case input::StopLoop:
            return on_stop();
        case input::SaveLoop:
            return on_save();
        case input::FirstChord:
            return on_first();
        case input::NextChord:
            return on_next();
        case input::PrevChord:
            return on_prev();
        case input::LastChord:
            return on_last();
        case input::SelectLoop:
            return on_select_loop(event.button_pressed.loop_number);
        case input::Run:
            return on_run();
        default: return; // so clang-tidy will shut up
    }
}

void Sequencer::handle_event(synth::SynthGetFrequenciesEvent& event) {
    // add from staged node
    if (_curr_loop != 0)
        _staged_node.add_to_synth_event(event);

    // add all running nodes
    for (uint8_t i = 0; i < NUM_TRACKS; ++i) {
        if (!_tracks[i].running) continue;
        _tracks[i].curr->data.add_to_synth_event(event);
    }
}

void Sequencer::on_start() {
    _staged_node.clear();
    if (_curr_loop == 0) return;
    _tracks[_curr_loop].running = true;

    set_running(true);
    set_loop(0);
}

void Sequencer::on_stop() {
    _staged_node.clear();
    if (_curr_loop == 0) return;

    _tracks[_curr_loop].running = false;
    set_loop(0);
}

void Sequencer::on_save() {
    commit_staged();
    // clear even if it fails
    _staged_node.clear();
}

void Sequencer::on_first() {
    set_first();
    _staged_node.clear();
}

void Sequencer::on_last() {
    set_last();
    _staged_node.clear();
}

void Sequencer::on_next() {
    add_beat_uncapped();
    _staged_node.clear();
}

void Sequencer::on_prev() {
    sub_beat_or_cancel();
    _staged_node.clear();
}

void Sequencer::on_select_loop(uint8_t loop_num) {
    // set to pressed button unless that button is already selected
    set_loop(_curr_loop == loop_num ? 0 : loop_num);
    _staged_node.clear();
    set_running(false);

    if (_curr_loop != 0)
        _tracks[_curr_loop-1].running = true;
}

void Sequencer::on_run() {
    set_running(!_run);
}

} // namespace sequencer
