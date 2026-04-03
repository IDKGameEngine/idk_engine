#include "idk/engine/engine.hpp"
#include "idk/core/assert.hpp"
#include "idk/core/log.hpp"

#include <atomic>
#include <cstring>
#include <filesystem>
#include <SDL3/SDL.h>


idk::Engine::Engine(uint32_t numServices)
:   num_services_(numServices),
    mainloop_sync_(numServices),
    shutdown_sync_(numServices)
{
    if (false == SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_CAMERA))
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
    std::filesystem::current_path(std::filesystem::path(SDL_GetBasePath()));

    running_.store(true);
    VLOG_INFO("Engine Initialized");
}


idk::Engine::~Engine()
{
    VLOG_INFO("[Engine::~Engine]");
}


void idk::Engine::addService(idk::core::Service *srv)
{
    services_.push_back(srv);
    threads_.push_back(std::thread(idk::Engine::_srvmain, this, srv));
}


void idk::Engine::start(idk::core::Service *mainsrv)
{
    services_.push_back(mainsrv);

    IDK_ASSERT(
        services_.size() == num_services_,
        "Supplied incorrect number of services"
    );

    for (auto &t: threads_)
    {
        t.detach();
    }

    while (this->running())
    {
        mainsrv->onUpdate(this);
        mainloop_sync_.arrive_and_wait();
    }

    shutdown_sync_.arrive_and_wait();
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

