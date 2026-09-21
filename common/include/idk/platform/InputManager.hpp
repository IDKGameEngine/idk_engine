#pragma once

#include "libidk/Service.hpp"


namespace idk
{
    class InputManager: public idk::Service
    {
    private:

    public:
        virtual void onInit(EngineAPI&) final;
        virtual void onShutdown(EngineAPI&) final;
        virtual void onPreFrame(EngineAPI&) final;
        virtual void onPostFrame(EngineAPI&) final;
        virtual void onEvent(EngineAPI&, const void*) final;

        bool isKeyDown(int keycode) const;
        bool isMouseButtonDown(int button) const;
        void getMousePosition(int& x, int& y) const;
    };

}

