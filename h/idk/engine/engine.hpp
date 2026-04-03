#pragma once

#include "idk/core/engine.hpp"
#include "idk/core/service.hpp"

#include <barrier>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>

namespace idk
{
    class Engine;
}

class idk::Engine: public idk::IEngine
{
public:
    Engine(uint32_t numServices);
    ~Engine();
    void addService(idk::core::Service*);
    void start(idk::core::Service *mainsrv);
    void shutdown();
    bool running();
    // bool set_ctrl(EngineCtrl);
    // EngineStat get_stat();

private:
    std::atomic_bool running_;

    std::uint32_t  num_services_;
    std::barrier<> mainloop_sync_;
    std::barrier<> shutdown_sync_;

    std::vector<core::Service*> services_;
    std::vector<std::thread>    threads_;

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

