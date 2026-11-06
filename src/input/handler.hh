#pragma once

#include <cstdint>

namespace input {

class InputHandler {
    friend class InputTimer;

public:
    virtual ~InputHandler() = default;

protected:
    virtual void update(uint32_t cycles) = 0;
};

} // namespace input
