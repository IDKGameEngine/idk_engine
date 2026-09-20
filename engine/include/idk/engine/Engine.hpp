#pragma once

#include "idk/engine/IApplication.hpp"
#include "idk/platform/PlatformContext.hpp"
#include "libidk/Service.hpp"
#include "libidk/dsa/Queue.hpp"


namespace idk
{
    struct EngineEvent;

    class Engine: public idk::ServiceManager
    {
    public:
        Engine();
        void run(idk::IApplication *app);

    private:
        std::atomic<bool> mShouldQuit;
        void process_engine_event(const EngineEvent&);

    };
}
