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
        bool x;
        bool y;
        bool z;
    };

    struct EngineStateData
    {
        EngineControlData controlPrev, controlCurr;
        EngineStatusData statusPrev, statusCurr;
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
        idk::PeriodicTimer mControlTimer;
        idk::PeriodicTimer mStatusTimer;
        idk::RemoteRxer mCtrlRx;
        idk::RemoteTxer  mStatTx;
        // idk::PortRxer<idk::RemoteRxer, EngineControlData> mCtrlRx;
        // idk::PortTxer<idk::RemoteTxer, EngineStatusData>  mStatTx;

        virtual core::Service *_getService(idk::IdType id) final;

    };

}

