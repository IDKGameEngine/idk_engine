#pragma once

#include "idk/EngineAPI.hpp"
#include "idk/EngineClock.hpp"
#include "libidk/dsa/PeriodicTimer.hpp"

namespace idk
{
    struct EngineEvent;

    class Engine: public idk::ServiceManager
    {
    public:
        Engine();
        void setFixedUpdateRate(double rateHz);
        void startApplication(idk::Service *app);

    private:
        std::atomic<bool>       mShouldQuit;
        idk::EngineAPI          mApi;
        idk::EngineClock        mClock{};
        idk::FixedAccumulator   mFixedAccumulator;

        void processEvents();
        void processEvent(const EngineEvent&);

    };
}
