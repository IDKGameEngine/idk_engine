#include "idk_engine/LocalGameService.hpp"
#include <SDL3_net/SDL_net.h>


static void LocalGameServiceRaiiFunc()
{
    if (!NET_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}


idk::LocalGameService::LocalGameService()
:   Service("LocalGameService", idk_typeid<LocalGameService>()),
    mRaii(LocalGameServiceRaiiFunc),
    mLocalClient()
{
    
}


