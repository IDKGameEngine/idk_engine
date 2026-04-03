#include "idk/engine/engine.hpp"
#include "idk/core/log.hpp"

#include <cstring>
#include <atomic>

void idk::Engine::_srvmain(idk::Engine *engine, idk::core::Service *srv)
{
    while (engine->running())
    {
        srv->onUpdate(engine);
        engine->mainloop_sync_->arrive_and_wait();
    }

    engine->shutdown_sync_->arrive_and_wait();
    srv->onShutdown(engine);
}


idk::Engine::Engine()
{
    running_.store(true);
    VLOG_INFO("Engine Initialized");
}


void idk::Engine::addService(idk::core::Service *srv)
{
    services_.push_back(srv);
    threads_.push_back(std::thread(idk::Engine::_srvmain, this, srv));
}


void idk::Engine::start(idk::core::Service *mainsrv)
{
    services_.push_back(mainsrv);
    mainloop_sync_ = new std::barrier(services_.size());
    shutdown_sync_ = new std::barrier(services_.size());

    for (auto &t: threads_)
    {
        t.detach();
    }

    while (this->running())
    {
        mainsrv->onUpdate(this);
        mainloop_sync_->arrive_and_wait();
    }

    shutdown_sync_->arrive_and_wait();
    mainsrv->onShutdown(this);

    for (size_t i=0; i<services_.size(); i++)
    {
        delete services_[i];
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

