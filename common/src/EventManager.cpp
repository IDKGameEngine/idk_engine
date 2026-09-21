#include "idk/platform/EventManager.hpp"
#include "idk/EngineAPI.hpp"

#include <SDL3/SDL.h>


idk::EventManager::EventManager()
{
    if (false == SDL_Init(SDL_INIT_EVENTS))
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}


void idk::EventManager::onInit(idk::EngineAPI&)
{

}


void idk::EventManager::onUpdate(idk::EngineAPI &api)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        for (uintptr_t addr: mEventCallbacks)
        {
            reinterpret_cast<EventCallback>(addr)(&e);
        }
        if (e.type == SDL_EVENT_QUIT)
        {
            api.pushEvent(EngineEvent::T_EngineCtl, EngineEvent::S_Shutdown);
        }
        else if ((e.type == SDL_EVENT_KEY_UP) && (e.key.scancode == SDL_SCANCODE_ESCAPE))
        {
            api.pushEvent(EngineEvent::T_EngineCtl, EngineEvent::S_Shutdown);
        }
        else
        {
            api.broadcastEvent(EngineEvent::T_Platform, 0, reinterpret_cast<uint64_t>(&e));
        }
    }
}

