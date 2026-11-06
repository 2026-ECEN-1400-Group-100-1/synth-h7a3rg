#pragma once

namespace input {

class InputHandler {
    friend class InputTimer;

public:
    virtual ~InputHandler() = default;

protected:
    virtual void update() = 0;
};

} // namespace input
