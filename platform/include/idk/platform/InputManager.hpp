#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class InputManager: public idk::IPlatformFeature
    {
    private:

    public:
        InputManager();

        virtual void onUpdate(idk::PlatformContext&) final;
        virtual void onEvent(idk::PlatformContext&, const void*) final;

        bool isKeyDown(int keycode) const;
        bool isMouseButtonDown(int button) const;
        void getMousePosition(int& x, int& y) const;
    };

}

