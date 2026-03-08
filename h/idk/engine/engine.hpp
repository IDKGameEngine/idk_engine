#pragma once

#include "idk/core/service.hpp"
#include "idk/core/renderer.hpp"
#include <cstdint>


namespace idk
{
    class Engine;

    enum class EngineStatus: uint8_t
    {
        Invalid = 0,
        Off,
        StartupInProgress,
        Running,
        ShutdownInProgress
    };

    enum class EngineControl: uint8_t
    {
        None = 0,
        BeginStartup,
        BeginShutdown
    };

}


class idk::Engine: public idk::NonCopyable, public idk::NonMovable
{
public:
    struct Impl;
    idk::Engine::Impl *m_impl;

    Engine(const idk::core::WindowDesc &windesc);
    ~Engine();

    void update();

    idk::EngineStatus  getStatus();
    idk::EngineControl getControl();
    bool setControl(idk::EngineControl);
    void awaitControl(idk::EngineControl c) { while (c != getControl()) {}; }
    void awaitStatus(idk::EngineStatus s) { while (s != getStatus()) {}; }

    // template <typename T> T get();
    // template <idk::EngineStatus> auto get<idk::EngineStatus>() { return getStatus(); }
    // template <idk::EngineControl> auto get<idk::EngineControl>() { return getControl(); }
    // template <idk::EngineControl C> bool set() { return setControl(C); }
    // template <idk::EngineStatus S> void await() { while (S != getStatus()) {  }; }
    // template <idk::EngineControl C> void await() { while (C != getControl()) {  }; }

    idk::core::IPlatform *getPlatform();
    idk::core::IRenderer *getRenderer();

private:
    idk::core::ServiceManager m_services;
    idk::EngineStatus _onStatusInvalid();
    idk::EngineStatus _onStatusOff();
    idk::EngineStatus _onStatusStartupInProgress();
    idk::EngineStatus _onStatusRunning();
    idk::EngineStatus _onStatusShutdownInProgress();

    void _set_stat(idk::EngineStatus);
    void _set_ctrl(idk::EngineControl);
    bool _match_and_unset(idk::EngineControl expected);
    void _await_and_unset(idk::EngineControl expected);

};

