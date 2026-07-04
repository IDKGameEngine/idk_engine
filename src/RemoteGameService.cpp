#include "idk_engine/RemoteGameService.hpp"
#include <SDL3_net/SDL_net.h>


static void RemoteGameServiceRaiiFunc()
{
    if (!NET_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}


idk::RemoteGameService::RemoteGameService()
:   Service("RemoteGameService", idk_typeid<RemoteGameService>())
    // mRaii(RemoteGameServiceRaiiFunc),
{
    
}


