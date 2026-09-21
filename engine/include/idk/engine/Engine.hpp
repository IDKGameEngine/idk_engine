#pragma once

#include "idk/EngineAPI.hpp"
#include "idk/engine/IApplication.hpp"


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
        idk::EngineAPI    mApi;

        void process_engine_event(const EngineEvent&);

    };
}
