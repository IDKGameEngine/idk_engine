#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class InputManager: public idk::IPlatformFeature
    {
    private:

    public:
        InputManager();

        virtual void update(idk::IPlatformContext*) final;

        bool isKeyDown(int keycode) const;
        bool isMouseButtonDown(int button) const;
        void getMousePosition(int& x, int& y) const;
    };

}

