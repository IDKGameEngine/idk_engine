#include "idk_engine/Engine.hpp"
#include "idk_engine/IoService.hpp"
#include "idk_engine/LocalGameService.hpp"
#include "idk_engine/RemoteGameService.hpp"

#include "idk/core/assert.hpp"
#include "idk/core/log.hpp"
#include "idk/core/metric.hpp"

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
{
    srvs_.push_back(new idk::engine::IoService());
    srvs_.push_back(new idk::engine::LocalGameService());
    srvs_.push_back(new idk::engine::RemoteGameService());

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
}
