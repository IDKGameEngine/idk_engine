#include "idk/engine.hpp"
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
    running_.store(true);

    for (auto *srv: services)
    {
        srvs_.push_back(srv);
    }

    for (auto *srv: srvs_)
    {
        srv->startup(this);
    }

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
