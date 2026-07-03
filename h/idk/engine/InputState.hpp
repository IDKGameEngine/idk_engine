#pragma once

#include <glm/glm.hpp>

namespace idk::engine
{
    struct InputState
    {
        glm::vec3 moveDelta; // interpolated
        glm::vec3 lookDelta; // accumulated or interpolated

        bool jumpPressed;
        bool jumpHeld;
        bool firePressed;
        bool fireHeld;

        InputState()
        :   moveDelta(0.0f), lookDelta(0.0f),
            jumpPressed(false), jumpHeld(false),
            firePressed(false), fireHeld(false)
        {

        }
    };
}
