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
    Engine(core::Service *mainsrv, std::initializer_list<core::Service*> rest);
    ~Engine();

    void start();
    void shutdown();
    bool running();

private:
    std::atomic_bool running_;

    std::barrier<> mainloop_sync_;
    std::barrier<> shutdown_sync_;

    idk::core::Service *mainsrv_;
    std::vector<core::Service*> subsrvs_;
    std::vector<std::thread> threads_;

    idk::PeriodicTimer<15> timer_;


    // std::atomic<EngineStat> stat_;
    // std::atomic<EngineCtrl> ctrl_;
    // std::mutex ctrl_mutex_;

    // EngineStat _onStatInvalid();
    // EngineStat _onStatAlive();
    // EngineStat _onStatDead();
    // EngineStat _onStatStarting();
    // EngineStat _onStatStopping();

    // void _set_stat(EngineStat s) { stat_.store(s); }
    // void _set_ctrl(EngineCtrl c) { ctrl_.store(c); }
    // bool _match_and_unset(EngineCtrl expected);
    // void _await_and_unset(EngineCtrl expected);

    void _update();

    static void _srvmain(idk::Engine*, idk::core::Service*);

};

