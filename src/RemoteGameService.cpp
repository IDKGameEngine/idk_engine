#include "idk_engine/RemoteGameService.hpp"
#include <SDL3_net/SDL_net.h>


static void RemoteGameServiceRaiiFunc()
{
    if (!NET_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}

idk::engine::RemoteGameService::RemoteGameService()
:   IDK_SERVICE_CTOR(RemoteGameService),
    mRaii(RemoteGameServiceRaiiFunc)
{
    
}

void idk::engine::RemoteGameService::update(idk::IEngine*)
{

}

void idk::engine::RemoteGameService::shutdown(idk::IEngine*)
{

}

