#include "idk/engine.hpp"
#include "idk/core/assert.hpp"
#include "idk/core/log.hpp"
#include "idk/core/metric.hpp"

#include <atomic>
#include <cstring>
#include <SDL3/SDL.h>


idk::Engine::Engine(core::Service *mainsrv, std::initializer_list<core::Service*> rest)
:   running_(true),
    num_srvs_(1 + rest.size()),
    startup_sync_(num_srvs_),
    shutdown_sync_(num_srvs_),
    mainsrv_(mainsrv)
{
    for (auto *srv: rest)
    {
        subsrvs_.push_back(srv);
        threads_.push_back(std::thread(Engine::_srvmain, this, srv));
    }

    for (auto &t: threads_)
    {
        t.detach();
    }

    VLOG_INFO("Engine Initialized");
    Engine::_srvmain(this, mainsrv_);
}

idk::Engine::~Engine()
{
    VLOG_INFO("[Engine::~Engine]");
}


bool idk::Engine::running()
{
    return running_.load();
}

void idk::Engine::shutdown()
{
    running_.store(false);
    running_.notify_all();
}

void idk::Engine::await_startup()
{
    startup_sync_.arrive_and_wait();
}

void idk::Engine::await_shutdown()
{
    shutdown_sync_.arrive_and_wait();
}


void idk::Engine::_srvmain(idk::Engine *E, idk::core::Service *srv)
{
    E->await_startup();
    srv->startup(E);

    while (E->running())
    {
        srv->update(E);
        // E->mainloop_sync_.arrive_and_wait();
    }

    srv->shutdown(E);
    E->await_shutdown();

    delete srv;
}

