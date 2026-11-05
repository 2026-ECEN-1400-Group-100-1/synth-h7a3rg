#pragma once

namespace input {

class InputHandler {
public:
    virtual ~InputHandler() = default;

    virtual void update() = 0;
};

}
