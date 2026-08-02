#include "idk_engine/Engine.hpp"

#include "libidk/assert.hpp"
#include "libidk/log.hpp"
#include "libidk/Service.hpp"

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


idk::Engine::Engine(std::initializer_list<core::Service*> services)
:   mStateData(),
    mControlTimer(4),
    mStatusTimer(8),
    mCtrlRx(5001),
    mStatTx("127.0.0.1", 5002)
{
    for (auto *srv: services)
    {
        srvs_.push_back(srv);
    }
    running_.store(true);
    VLOG_INFO("Engine Initialized");
}


bool idk::Engine::running()
{
    return running_.load();
}


void idk::Engine::shutdown()
{
    running_.store(false);
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
        while (mCtrlRx.recvMsg(mStateData.controlCurr))
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
        mStateData.statusCurr.x = (mStateData.controlCurr.x == 1);
        mStateData.statusCurr.y = (mStateData.controlCurr.y == 1);
        mStateData.statusCurr.z = (mStateData.controlCurr.z == 1);
        mStatTx.sendMsg(mStateData.statusCurr);
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

