#include "idk/platform/EventManager.hpp"
#include "idk/platform/PlatformContext.hpp"
#include "libidk/log.hpp"

#include "idk/engine-message/EventQueue.hpp"

#include <SDL3/SDL.h>


idk::EventManager::EventManager(PlatformContext &ctx)
:   IPlatformFeature(ctx)
{
    if (false == SDL_Init(SDL_INIT_EVENTS))
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}


void idk::EventManager::onInit(idk::ServiceManager*)
{

}


void idk::EventManager::onUpdate(idk::ServiceManager*)
{
    auto &eq = idk::EngineEvent::gEngineEventQueue;

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        for (uintptr_t addr: mEventCallbacks)
        {
            reinterpret_cast<EventCallback>(addr)(&e);
        }
        if (e.type == SDL_EVENT_QUIT)
        {
            eq.push({EngineEvent::SHUTDOWN, 0});
        }
        else if ((e.type == SDL_EVENT_KEY_UP) && (e.key.scancode == SDL_SCANCODE_ESCAPE))
        {
            eq.push({EngineEvent::SHUTDOWN, 0});
        }
        else
        {
            mCtx.broadcastEvent(&e);
        }
    }
}

