#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class InputManager: public idk::IPlatformFeature
    {
    private:

    public:
        InputManager();
        virtual ~InputManager() = default;

        virtual void onUpdate(EngineAPI&) final;
        virtual void onShutdown(EngineAPI&) final;
        virtual void onEvent(EngineAPI&, const void*) final;

        bool isKeyDown(int keycode) const;
        bool isMouseButtonDown(int button) const;
        void getMousePosition(int& x, int& y) const;
    };

}

