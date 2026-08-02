#include "idk_engine/Engine.hpp"

#include "libidk/message/RemoteRxTx.hpp"
#include "libidk/message/SharedRxTx.hpp"

#include "libidk/assert.hpp"
#include "libidk/log.hpp"
#include "libidk/Service.hpp"
#include "libidk/New.hpp"

#include <atomic>
#include <cstring>
#include <SDL3/SDL.h>

idk::EngineConfig &idk::IEngine::getConfig()
{
    static EngineConfig instance;
    return instance;
}

idk::CfgParser &idk::IEngine::getCfgParser()
{
    return IEngine::getConfig().cfgParser;
}

static void EngineRaiiFunc()
{
    idk::IEngine::getCfgParser().load("Engine.cfg");
}

// static size_t EngineOnRecvCtrl(void *req, size_t reqsz, void *res)
// {
//     return 0;
// }


idk::Engine::Engine(idk::platform::Platform &plat, std::initializer_list<core::Service*> services)
:   mPlat(plat),
    mRaii(EngineRaiiFunc),
    mCfg(IEngine::getCfgParser()["Engine"]),
    mStateData(),
    mStatusData(),
    mControlTimer(4),
    mStatusTimer(8),
    mCtrlRx(idk::New<RemoteRxer>(mCfg["CTRL_PORT"].toU16())),
    mStatTx(idk::New<RemoteTxer>("192.168.0.11", 5002))
    // mStatTx(idk::New<RemoteTxer>("127.0.0.1", 5002))
    // mCtrlRx(idk::New<SharedRxer>("IDKGameEngineIPC-EngineControl", sizeof(EngineControlData))),
    // mStatTx(idk::New<SharedTxer>("IDKGameEngineIPC-EngineStatus", sizeof(EngineStatusData)))
{
    for (auto *srv: services)
    {
        srvs_.push_back(srv);
    }
    // running_.store(true);
    // ((RemoteRxer*)mCtrlRx)->onRecvMsg = EngineOnRecvCtrl;

    VLOG_INFO("Engine Initialized");
}


bool idk::Engine::running()
{
    return mPlat.getStatus() == platform::Status::Running;
}


void idk::Engine::shutdown()
{
    mPlat.shutdown();
}


void idk::Engine::update()
{
    for (auto *srv: srvs_)
    {
        srv->update(this);
    }

    if (!running())
    {
        for (auto *srv: srvs_)
        {
            srv->shutdown(this);
        }
    }

    if (mControlTimer.expired())
    {
        mControlTimer.reset();
        while (mCtrlRx->recvMsg(mStateData.controlCurr))
        {
            auto &prev = mStateData.controlPrev;
            auto &curr = mStateData.controlCurr;
            if (curr.x != prev.x) { VLOG_INFO("ctrl.x: {} -> {}", prev.x, curr.x); }
            if (curr.y != prev.y) { VLOG_INFO("ctrl.y: {} -> {}", prev.y, curr.y); }
            if (curr.z != prev.z) { VLOG_INFO("ctrl.z: {} -> {}", prev.z, curr.z); }
            mStateData.controlPrev = mStateData.controlCurr;
        }
    }

    if (mStatusTimer.expired())
    {
        mStatusTimer.reset();
        mStatusData.allocatorMemoryUsage = idk::GetAllocatorMemoryUsage();
        mStatusData.x = mStateData.controlCurr.x;
        mStatusData.y = mStateData.controlCurr.y;
        mStatusData.z = mStateData.controlCurr.z;
        mStatTx->sendMsg(mStatusData);
    }
}


idk::core::Service *idk::Engine::_getService(idk::IdType id)
{
    for (idk::core::Service *srv: srvs_)
    {
        if (srv->getTypeId() == id)
        {
            return srv;
        }
    }
    return nullptr;
}

