#pragma once

#include "handler.hh"

namespace input {

class KeyHandler final : public InputHandler {
public:
    explicit KeyHandler() {}

    void update() override;
};

} // namespace input
