#pragma once

#include "idk/core/engine.hpp"
#include "idk/core/service.hpp"
#include "idk/core/periodic_timer.hpp"

#include <atomic>
#include <barrier>
#include <initializer_list>
#include <mutex>
#include <thread>
#include <vector>

namespace idk
{
    class Engine;
}

class idk::Engine: public idk::IEngine
{
public:
    Engine(std::initializer_list<core::Service*> mainthread_srvs, std::initializer_list<core::Service*> workthread_srvs);
    ~Engine();
    virtual bool running() final;
    virtual void shutdown() final;
    virtual void await_startup() final;
    virtual void await_shutdown() final;

private:
    std::atomic_bool running_;
    const size_t     num_srvs_;

    std::barrier<>   startup_sync_;
    std::barrier<>   shutdown_sync_;

    std::vector<std::thread> workthreads_;

    static void _workthread_main(idk::Engine*, idk::core::Service*);

};

