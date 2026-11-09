#pragma once

#define MAX_EVENT_SUBSCRIBERS 16

namespace utils {

template <typename T> class Event;

template <typename T>
class EventSubscriber {
    friend class Event<T>;

public:
    EventSubscriber() { Event<T>::subscribe(*this); }

    virtual ~EventSubscriber() = default;

protected:
    virtual void handle_event(T& event) = 0;
};

template <typename T>
class Event {
public:
    explicit Event(T& wrapped) : _wrap(wrapped) {}

    static bool subscribe(EventSubscriber<T>& subscriber);
    void raise();

    T& args() { return _wrap; }

private:
    static inline EventSubscriber<T>* _subscribers[MAX_EVENT_SUBSCRIBERS] = {};
    static inline uint8_t _num_subscribers = 0;

    T& _wrap;
};

template<typename T>
bool Event<T>::subscribe(EventSubscriber<T>& subscriber) {
    if (_num_subscribers >= MAX_EVENT_SUBSCRIBERS)
        return false;

    _subscribers[_num_subscribers++] = &subscriber;
    return true;
}

template<typename T>
void Event<T>::raise() {
    for (uint8_t i = 0; i < _num_subscribers; ++i) {
        _subscribers[i]->handle_event(_wrap);
    }
}

} // namespace utils
