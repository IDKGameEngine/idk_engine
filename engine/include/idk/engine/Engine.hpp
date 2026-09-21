#pragma once

#include "idk/EngineAPI.hpp"
#include "idk/EngineClock.hpp"
#include "libidk/dsa/Raii.hpp"
#include "libidk/dsa/Timer.hpp"

namespace idk
{
    struct EngineEvent;

    class Engine: public idk::ServiceManager
    {
    public:
        Engine();
        ~Engine();

        void setFixedUpdateRate(double rateHz);
        void startApplication(idk::Service *app);

    private:
        idk::RaiiFunc<void(Engine*)> mServiceRaii;
        idk::EngineAPI               mApi;
        idk::EngineClock             mClock{};
        idk::FixedTimer              mFixedTimer;

        void engineDeltaTime();
        void engineEvents();
        void engineUpdate();
        void engineRender();

        void processEvent(const EngineEvent&);

        static void onEngineEventCallback(EngineAPI&, const EngineEvent&);

    };
}
