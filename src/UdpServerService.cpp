#include "idk/UdpServerService.hpp"
#include "idk/core/log.hpp"
#include "idk/core/platform.hpp"
#include "idk/core/stdmem.hpp"

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>


static void UdpServerServiceRaiiFunc()
{
    if (!NET_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}


idk::UdpServerService::UdpServerService()
:   Service("UdpServerService", idk_typeid<UdpServerService>()),
    mRaii(UdpServerServiceRaiiFunc)
{
    
}


void idk::UdpServerService::update(idk::IEngine*)
{

}


void idk::UdpServerService::shutdown(idk::IEngine*)
{
    VLOG_INFO("[UdpServerService::shutdown]");
    NET_Quit();
}

