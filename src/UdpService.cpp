#include "idk/UdpService.hpp"
#include "idk/core/log.hpp"
#include "idk/core/stdmem.hpp"

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>


idk::UdpService::UdpService()
:   Service("UdpService", idk_typeid<UdpService>())
{
    if (!NET_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}


void idk::UdpService::startup(idk::IEngine*)
{
    VLOG_INFO("[UdpService::startup]");

    mRxers.emplace(mCfg["PORT_IO_CTRL"].getValueU16());
    mRxers.emplace(mCfg["PORT_UDP_CTRL"].getValueU16());
    mRxers.emplace(mCfg["PORT_ENGINE_CTRL"].getValueU16());
}


void idk::UdpService::update(idk::IEngine*)
{
    for (idk::UdpRxer &rxer: mRxers)
    {
        static constexpr int bufsz = 128;
        static char buf[bufsz];
        idk_memset(buf, 0, bufsz);

        if (0 != rxer.recvmsg(buf, bufsz))
        {
            VLOG_INFO("[UdpService::update] received msg: {}", buf);
        }
    }
}


void idk::UdpService::shutdown(idk::IEngine*)
{
    VLOG_INFO("[UdpService::shutdown]");
    NET_Quit();
}

