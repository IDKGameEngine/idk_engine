#pragma once

#include "idk/core/service.hpp"
#include "idk/core/renderer.hpp"
#include "idk/core/platform.hpp"
#include "idk/core/engine.hpp"

#include <atomic>
#include <mutex>


namespace idk
{
    class Engine;
}


class idk::Engine: public idk::core::IEngine
{
public:
    Engine();
    void update();

    virtual void shutdown() final;
    virtual bool set_ctrl(EngineCtrl) final;
    virtual EngineStat get_stat() final;

private:
    std::atomic<EngineStat> stat_;
    std::atomic<EngineCtrl> ctrl_;
    std::mutex ctrl_mutex_;

    EngineStat _onStatInvalid();
    EngineStat _onStatAlive();
    EngineStat _onStatDead();
    EngineStat _onStatStarting();
    EngineStat _onStatStopping();

    void _set_stat(EngineStat s) { stat_.store(s); }
    void _set_ctrl(EngineCtrl c) { ctrl_.store(c); }
    bool _match_and_unset(EngineCtrl expected);
    void _await_and_unset(EngineCtrl expected);

};

