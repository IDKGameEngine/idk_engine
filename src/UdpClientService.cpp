#include "idk/UdpClientService.hpp"
#include "idk/core/log.hpp"
#include "idk/core/platform.hpp"
#include "idk/core/stdmem.hpp"

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>


static void UdpClientServiceRaiiFunc()
{
    if (!NET_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}


idk::UdpClientService::UdpClientService()
:   Service("UdpClientService", idk_typeid<UdpClientService>()),
    mRaii(UdpClientServiceRaiiFunc),
    mTimeSyncRxer(),
    mTimeSyncTxer()
{

}


void idk::UdpClientService::update(idk::IEngine*)
{
    config::udp::TimeSyncData timeSyncData;

    if (mTimeSyncRxer.recvmsg(timeSyncData))
    {
        uint64_t clientSendTime = timeSyncData.clientSendTime;
        VLOG_INFO("TimeSyncData::clientSendTime: {}", clientSendTime);
        timeSyncData.serverSendTime = idk::platform::GetSysTimeMs();
        mTimeSyncTxer.sendmsg(timeSyncData);
    }
    // for (idk::UdpRxer &rxer: mRxers)
    // {
    //     static constexpr int bufsz = 128;
    //     static char buf[bufsz];
    //     idk_memset(buf, 0, bufsz);

    //     if (0 != rxer.recvmsg(buf, bufsz))
    //     {
    //         VLOG_INFO("[UdpClientService::update] received msg: {}", buf);
    //     }
    // }
}


void idk::UdpClientService::shutdown(idk::IEngine*)
{
    VLOG_INFO("[UdpClientService::shutdown]");
    NET_Quit();
}

