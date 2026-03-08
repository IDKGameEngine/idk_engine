#include "idk/engine/engine.hpp"
#include "idk/platform/platform.hpp"
#include "idk/gfx/gfx.hpp"
#include <atomic>

using EngineFptr = idk::EngineStatus(idk::Engine::*)(void);
static EngineFptr ftab_[5];


struct idk::Engine::Impl
{
    idk::core::IPlatformPtr         plat;
    idk::core::IRendererPtr         ren;
    std::atomic<idk::EngineStatus>  stat;
    std::atomic<idk::EngineControl> ctrl;

    Impl(const idk::core::WindowDesc &windesc)
    :   plat( idk::platform::create_platform(windesc) ),
        ren ( idk::gfx::create_renderer(plat->getWindow()) )
    {
        stat.store(idk::EngineStatus::Off);
        ctrl.store(idk::EngineControl::None);
    }

};


static void onServiceShutdown(void *arg)
{
    VLOG_INFO("idk::Engine onServiceShutdown");

    idk::Engine *e = static_cast<idk::Engine*>(arg);

    IDK_ASSERT(
        e->getStatus() == idk::EngineStatus::Running,
        "Engine must first be running in order shutdown"
    );

    e->setControl(idk::EngineControl::BeginShutdown);
}


idk::Engine::Engine(const idk::core::WindowDesc &wd)
:   idk::NonCopyable(),
    idk::NonMovable(),
    m_impl(new idk::Engine::Impl(wd))
{
    ftab_[0] = &idk::Engine::_onStatusInvalid;
    ftab_[1] = &idk::Engine::_onStatusOff;
    ftab_[2] = &idk::Engine::_onStatusStartupInProgress;
    ftab_[3] = &idk::Engine::_onStatusRunning;
    ftab_[4] = &idk::Engine::_onStatusShutdownInProgress;

    m_services.shutdown_func_ = onServiceShutdown;
    m_services.shutdown_arg_ = (void*)this;

    VLOG_INFO("Engine Initialized");

    this->setControl(EngineControl::BeginStartup);
    this->update();
}

idk::Engine::~Engine()
{
    delete m_impl;
}


idk::core::IPlatform *idk::Engine::getPlatform()
{
    return m_impl->plat.get();
}


idk::core::IRenderer *idk::Engine::getRenderer()
{
    return m_impl->ren.get();
}



void idk::Engine::update()
{
    int idx = 0;
    EngineStatus stat = getStatus();

    switch (stat)
    {
        default:
        case EngineStatus::Invalid:
            idx = 0;
            break;

        case EngineStatus::Off:
        case EngineStatus::StartupInProgress:
        case EngineStatus::Running:
        case EngineStatus::ShutdownInProgress:
            idx = static_cast<int>(stat);
            break;
    }

    _set_stat((this->*(ftab_[idx]))());
}


idk::EngineStatus idk::Engine::_onStatusInvalid()
{
    VLOG_FATAL("Invalid EngineStatus");
    return EngineStatus::Invalid;
}


idk::EngineStatus idk::Engine::_onStatusOff()
{
    VLOG_INFO("Engine::_onStatusOff");
    if (_match_and_unset(EngineControl::BeginStartup))
        return EngineStatus::StartupInProgress;
    return EngineStatus::Off;
}


idk::EngineStatus idk::Engine::_onStatusStartupInProgress()
{
    VLOG_INFO("Engine::_onStatusStartupInProgress");
    m_services.registerService(m_impl->plat);
    m_services.registerService(m_impl->ren);
    return EngineStatus::Running;
}


idk::EngineStatus idk::Engine::_onStatusRunning()
{
    if (_match_and_unset(EngineControl::BeginShutdown))
        return EngineStatus::ShutdownInProgress;
    m_services.update();
    return EngineStatus::Running;
}


idk::EngineStatus idk::Engine::_onStatusShutdownInProgress()
{
    VLOG_INFO("Engine::_onStatusShutdownInProgress");
    m_services.shutdown();
    return EngineStatus::Off;
}



void idk::Engine::_set_stat(idk::EngineStatus  s) { m_impl->stat.store(s); }
void idk::Engine::_set_ctrl(idk::EngineControl c) { m_impl->ctrl.store(c);  }

/*
    atomic<int> foo;

    int expected = 3;
    while(!foo.compare_exchange_strong(expected, 4))
    {
        // somebody beat us to it
        // now decided what to do; break the loop? try again?
        if (expected == 4)
        {
            // somebody already wrote a 4 there
            break;
        } // otherwise keep trying
    }
*/
bool idk::Engine::_match_and_unset(idk::EngineControl expected)
{
    return m_impl->ctrl.compare_exchange_strong(expected, EngineControl::None);
}

void idk::Engine::_await_and_unset(idk::EngineControl expected)
{
    auto &ref = m_impl->ctrl;

    while (!ref.compare_exchange_strong(expected, EngineControl::None))
    {
        
    }
}



idk::EngineStatus idk::Engine::getStatus()
{
    return m_impl->stat.load();
}

// idk::EngineControl idk::Engine::getControl()
// {
//     return m_impl->ctrl.load();
// }

bool idk::Engine::setControl(idk::EngineControl ctl)
{
    switch (getStatus())
    {
        case EngineStatus::Off:
        case EngineStatus::Running:
            m_impl->ctrl.store(ctl);
            return true;

        case EngineStatus::StartupInProgress:
        case EngineStatus::ShutdownInProgress:
            return false;

        default:
            VLOG_FATAL("Invalid EngineStatus");
            return false;
    }
}
