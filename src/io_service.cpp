#include "idk/io_service.hpp"
#include "idk/core/log.hpp"

#include <SDL3/SDL.h>


idk::IoService::IoService()
:   Service(idk_typeid<IoService>())
{

}


void idk::IoService::startup(idk::IEngine*)
{
    VLOG_INFO("[idk::IoService::startup]");
}


void idk::IoService::update(idk::IEngine *E)
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


void idk::IoService::shutdown(idk::IEngine*)
{
    VLOG_INFO("[idk::IoService::_shutdown]");
}

