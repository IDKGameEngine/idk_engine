#pragma once

#include "idk_engine/Engine.hpp"
#include "idk/core/metric.hpp"

struct SDL_Window;

namespace idk::engine
{
    class PlatformService: public idk::core::IPlatformService
    {
    public:
        PlatformService(bool headless = false);
        ~PlatformService();
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;
        virtual void getWindowSize(int &w, int &h) final;
        virtual void swapWindow() final;

    private:
        SDL_Window *mWin;
        void *mGl;

    };
}

