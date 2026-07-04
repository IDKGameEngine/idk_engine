#include "idk_engine/IoService.hpp"
#include "idk/core/log.hpp"

#include <SDL3/SDL.h>


idk::engine::IoService::IoService()
:   IDK_SERVICE_CTOR(IoService)
{

}


void idk::engine::IoService::startup(idk::IEngine*)
{
    VLOG_INFO("[idk::IoService::startup]");
}


void idk::engine::IoService::update(idk::IEngine *E)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            VLOG_INFO("SDL_EVENT_QUIT");
            E->shutdown();
        }

        if (e.type == SDL_EVENT_KEY_UP)
        {
            if (e.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                E->shutdown();
            }
        }
    }
}


void idk::engine::IoService::shutdown(idk::IEngine*)
{
    VLOG_INFO("[idk::IoService::_shutdown]");
}

