#pragma once

#include "libidk/Engine.hpp"
#include "libidk/Service.hpp"
#include "libidk/message/LocalRxTx.hpp"
#include "libidk/message/RemoteRxTx.hpp"
#include "libidk/message/PortRxTx.hpp"

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
        Engine(std::initializer_list<core::Service*> services);
        virtual bool running() final;
        virtual void shutdown() final;
        virtual void update() final;

    private:
        std::atomic<bool> running_;
        std::vector<core::Service*> srvs_;
        EngineStateData mStateData;
        EngineStatusData mStatusData;
        idk::PeriodicTimer mControlTimer;
        idk::PeriodicTimer mStatusTimer;
        idk::RemoteRxer mCtrlRx;
        idk::RemoteTxer mStatTx;
        // idk::SharedTxer mStatTx;

        virtual core::Service *_getService(idk::IdType id) final;

    };

}

