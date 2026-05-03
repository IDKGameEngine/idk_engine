#include "idk/engine.hpp"
#include "idk/core/assert.hpp"
#include "idk/core/log.hpp"
#include "idk/core/metric.hpp"

#include <atomic>
#include <cstring>
#include <SDL3/SDL.h>



idk::Engine::Engine(std::initializer_list<core::Service*> mainsrvs, std::initializer_list<core::Service*> worksrvs)
:   running_(true),
    num_srvs_(1 + worksrvs.size()),
    startup_sync_(num_srvs_),
    shutdown_sync_(num_srvs_)
{
    for (auto *srv: worksrvs)
    {
        workthreads_.push_back(std::thread(Engine::_workthread_main, this, srv));
        workthreads_.back().detach();
    }

    VLOG_INFO("Engine Initialized");

    this->await_startup();
    for (auto *srv: mainsrvs)
        srv->startup(this);

    while (this->running())
        for (auto *srv: mainsrvs)
            srv->update(this);

    for (auto *srv: mainsrvs)
        srv->shutdown(this);
    this->await_shutdown();

    for (auto *srv: mainsrvs)
        delete srv;
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


void idk::Engine::_workthread_main(idk::Engine *E, idk::core::Service *srv)
{
    E->await_startup();
    srv->startup(E);

    while (E->running())
    {
        srv->update(E);
    }

    srv->shutdown(E);
    E->await_shutdown();

    delete srv;
}

