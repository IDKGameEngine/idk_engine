#include "idk_engine/Engine.hpp"

#include "libidk/message/RemoteRxTx.hpp"

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
    mCtrl(),
    mStat(),
    mCtrlTimer(32),
    mStatTimer(32),
    mCtrlRxTx(nullptr, mCfg["CTRL_PORT"].toU16())
    // mStatPort(mCfg["STAT_PORT"].toU16())
    // mStatTx("127.0.0.1", mCfg["STAT_PORT"].toU16())
{
    for (auto *srv: services)
    {
        srvs_.push_back(srv);
    }
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

    if (mCtrlTimer.expired())
    {
        mCtrlTimer.reset();
        while (auto *msg = mCtrlRxTx.recvMsg())
        {
            handleCtrlMessage(msg);
        }
    }

    // if (mStatTimer.expired())
    // {
    //     mStatTimer.reset();

    //     mStatTx->sendMsg(mStat);
    // }
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


void idk::Engine::handleCtrlMessage(idk::MessageRecvInfo *msg)
{
    auto &h = msg->header;
    if (!msg->isType("CTRL"))
    {
        VLOG_WARN("[Engine::handleCtrlMessage] Recieved \"{}\"", h.payloadType.ascii);
        return;
    }
    if (h.payloadSize != sizeof(EngineCtrlData))
    {
        VLOG_WARN("[Engine::handleCtrlMessage] payloadSize != sizeof(EngineCtrlData)");
        return;
    }
    idk_memcpy(&mCtrl, msg->payload, h.payloadSize);

    auto &prev = mStateData.controlPrev;
    if (mCtrl.x != prev.x) { VLOG_INFO("ctrl.x: {} -> {}", prev.x, mCtrl.x); }
    if (mCtrl.y != prev.y) { VLOG_INFO("ctrl.y: {} -> {}", prev.y, mCtrl.y); }
    if (mCtrl.z != prev.z) { VLOG_INFO("ctrl.z: {} -> {}", prev.z, mCtrl.z); }
    prev = mCtrl;

    mStat.allocatorMemoryUsage = idk::GetAllocatorMemoryUsage();
    mStat.x = mCtrl.x;
    mStat.y = mCtrl.y;
    mStat.z = mCtrl.z;

    if (mCtrl.kill)
    {
        this->shutdown();
    }

    mCtrlRxTx.sendMsg(&mStat, sizeof(mStat), "STAT");
}

