#pragma once

#include "libidk/Engine.hpp"
#include "libidk/Service.hpp"
#include "libidk/message/RemoteRxTx.hpp"

#include <atomic>
#include <initializer_list>

namespace idk
{
    struct EngineCtrlData
    {
        bool kill;
        int32_t x;
        int32_t y;
        int32_t z;
    };

    struct EngineStatData
    {
        float allocatorMemoryUsage;
        int32_t x;
        int32_t y;
        int32_t z;
    };

    struct EngineStateData
    {
        EngineCtrlData controlPrev, controlCurr;
    };

    class Engine: public idk::IEngine
    {
    public:
        Engine(idk::platform::Platform &plat, std::initializer_list<core::Service*> services);
        virtual bool running() final;
        virtual void shutdown() final;
        virtual void update() final;

    private:
        idk::platform::Platform   &mPlat;
        RaiiFunc<void()>           mRaii;
        const CfgParser::TreeNode &mCfg;
        std::vector<core::Service*> srvs_;
        EngineStateData     mStateData;
        EngineCtrlData      mCtrl;
        EngineStatData      mStat;
        idk::PeriodicTimer  mCtrlTimer;
        idk::PeriodicTimer  mStatTimer;
        idk::RemoteRxer     mCtrlRx;
        uint16_t            mStatPort;
        // idk::RemoteTxer     mStatTx;

        virtual core::Service *_getService(idk::IdType id) final;
        void handleCtrlMessage();

    };

}

