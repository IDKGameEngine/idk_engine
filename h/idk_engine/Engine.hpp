#pragma once

#include "libidk/Engine.hpp"
#include "libidk/Service.hpp"
#include "libidk/message/MessageRxTx.hpp"

#include <atomic>
#include <initializer_list>

namespace idk
{
    struct EngineControlData
    {
        int32_t x;
        int32_t y;
        int32_t z;
    };

    struct EngineStatusData
    {
        float allocatorMemoryUsage;
        bool x;
        bool y;
        bool z;
    };

    struct EngineStateData
    {
        EngineControlData controlPrev, controlCurr;
    };

    class Engine: public idk::IEngine
    {
    public:
        Engine(idk::platform::Platform &plat, std::initializer_list<core::Service*> services);
        virtual bool running() final;
        virtual void shutdown() final;
        virtual void update() final;

    private:
        idk::platform::Platform &mPlat;
        std::vector<core::Service*> srvs_;
        EngineStateData mStateData;
        EngineStatusData mStatusData;
        idk::PeriodicTimer mControlTimer;
        idk::PeriodicTimer mStatusTimer;
        idk::MessageRxer *mCtrlRx;
        idk::MessageTxer *mStatTx;

        virtual core::Service *_getService(idk::IdType id) final;

    };

}

