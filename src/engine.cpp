#include "idk/engine/engine.hpp"
#include "idk/core/assert.hpp"
#include "idk/core/log.hpp"

#include <atomic>
#include <cstring>
#include <SDL3/SDL.h>


idk::Engine::Engine(core::Service *mainsrv, std::initializer_list<core::Service*> rest)
:   mainloop_sync_(rest.size() + 1),
    shutdown_sync_(rest.size() + 1),
    mainsrv_(mainsrv)
{
    running_.store(true);

    for (auto *srv: rest)
    {
        subsrvs_.push_back(srv);
        threads_.push_back(std::thread(idk::Engine::_srvmain, this, srv));
    }

    VLOG_INFO("Engine Initialized");
}


idk::Engine::~Engine()
{
    VLOG_INFO("[Engine::~Engine]");
}


// void idk::Engine::addService(idk::core::Service *srv)
// {
//     subsrvs_.push_back(srv);
//     threads_.push_back(std::thread(idk::Engine::_srvmain, this, srv));
// }


void idk::Engine::start()
{
    for (auto &t: threads_)
    {
        t.detach();
    }

    while (this->running())
    {
        mainsrv_->onUpdate(this);
        mainloop_sync_.arrive_and_wait();
    }

    shutdown_sync_.arrive_and_wait();
    mainsrv_->onShutdown(this);

    for (size_t i=0; i<subsrvs_.size(); i++)
    {
        delete subsrvs_[i];
    }
}


void idk::Engine::shutdown()
{
    running_.store(false);
}

bool idk::Engine::running()
{
    return running_.load();
}


void idk::Engine::_srvmain(idk::Engine *engine, idk::core::Service *srv)
{
    while (engine->running())
    {
        srv->onUpdate(engine);
        engine->mainloop_sync_.arrive_and_wait();
    }

    engine->shutdown_sync_.arrive_and_wait();
    srv->onShutdown(engine);
}

