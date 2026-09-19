#pragma once

namespace idk
{
    class CharacterController
    {
    public:
        virtual ~CharacterController() = default;
        virtual void attack() = 0;
        virtual void crouch() = 0;
        virtual void jump() = 0;
    };
}
