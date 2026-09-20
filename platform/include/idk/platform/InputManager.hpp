#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class InputManager: public idk::IPlatformFeature
    {
    private:

    public:
        InputManager(PlatformContext &ctx);

        virtual void onUpdate(ServiceManager*) final;
        virtual void onEvent(ServiceManager*, const void*) final;

        bool isKeyDown(int keycode) const;
        bool isMouseButtonDown(int button) const;
        void getMousePosition(int& x, int& y) const;
    };

}

