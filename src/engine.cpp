#include "idk/engine/engine.hpp"
#include "idk/platform/platform.hpp"
#include "idk/gfx/gfx.hpp"
#include <atomic>

using EngineFptr = idk::EngineStat(idk::Engine::*)(void);
static EngineFptr ftab_[5];

idk::Engine::Engine()
:   idk::core::IEngine(),
    stat_(EngineStat::Dead),
    ctrl_(EngineCtrl::None)
{
    ftab_[0] = &idk::Engine::_onStatInvalid;
    ftab_[1] = &idk::Engine::_onStatAlive;
    ftab_[2] = &idk::Engine::_onStatDead;
    ftab_[3] = &idk::Engine::_onStatStarting;
    ftab_[4] = &idk::Engine::_onStatStopping;

    VLOG_INFO("Engine Initialized");

    set_ctrl(EngineCtrl::Start);
    this->update();
}


void idk::Engine::update()
{
    int idx = 0;
    EngineStat stat = get_stat();

    switch (stat)
    {
        default:
        case EngineStat::Invalid:
            idx = 0;
            break;
        case EngineStat::Alive:
        case EngineStat::Dead:
        case EngineStat::Starting:
        case EngineStat::Stopping:
            idx = static_cast<int>(stat);
            break;
    }

    _set_stat((this->*(ftab_[idx]))());
}

void idk::Engine::shutdown()
{
    _set_ctrl(EngineCtrl::Stop);
    _set_stat(EngineStat::Stopping);
}



bool idk::Engine::set_ctrl(idk::EngineCtrl ctrl)
{
    std::lock_guard<std::mutex> lock(ctrl_mutex_);

    if (statIsCtrlEnabled(stat_.load()))
    {
        ctrl_.store(ctrl);
        return true;
    }

    return false;
}


idk::EngineStat idk::Engine::get_stat()
{
    return stat_.load();
}


idk::EngineStat idk::Engine::_onStatInvalid()
{
    VLOG_FATAL("Invalid EngineStat");
    return EngineStat::Invalid;
}

idk::EngineStat idk::Engine::_onStatAlive()
{
    if (_match_and_unset(EngineCtrl::Stop))
        return EngineStat::Stopping;
    return EngineStat::Alive;
}


idk::EngineStat idk::Engine::_onStatDead()
{
    VLOG_INFO("Engine::_onStatusOff");
    if (_match_and_unset(EngineCtrl::Start))
        return EngineStat::Starting;
    return EngineStat::Dead;
}


idk::EngineStat idk::Engine::_onStatStarting()
{
    VLOG_INFO("Engine::_onStatusStarting");
    return EngineStat::Alive;
}


idk::EngineStat idk::Engine::_onStatStopping()
{
    VLOG_INFO("Engine::_onStatusStopping");
    return EngineStat::Dead;
}



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
bool idk::Engine::_match_and_unset(idk::EngineCtrl expected)
{
    return ctrl_.compare_exchange_strong(expected, EngineCtrl::None);
}

void idk::Engine::_await_and_unset(idk::EngineCtrl expected)
{
    while (!ctrl_.compare_exchange_strong(expected, EngineCtrl::None))
    {
        
    }
}


