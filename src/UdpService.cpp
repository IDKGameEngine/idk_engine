#include "idk/UdpService.hpp"
#include "idk/core/log.hpp"
#include "idk/core/platform.hpp"
#include "idk/core/stdmem.hpp"

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>


static void UdpServiceRaiiFunc()
{
    if (!NET_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}


idk::UdpService::UdpService()
:   Service("UdpService", idk_typeid<UdpService>()),
    mRaii(UdpServiceRaiiFunc),
    mTimeSyncRxer(),
    mTimeSyncTxer()
{
    // mRxers.emplace(mCfg["PORT_IO_CTRL"].getValueU16());
    // mRxers.emplace(mCfg["PORT_UDP_CTRL"].getValueU16());
    // mRxers.emplace(mCfg["PORT_ENGINE_CTRL"].getValueU16());
    // mRxers.emplace(mCfg["PORT_TIME_SYNC"].getValueU16());
}


void idk::UdpService::startup(idk::IEngine*)
{
    VLOG_INFO("[UdpService::startup]");
}


void idk::UdpService::update(idk::IEngine*)
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
    //         VLOG_INFO("[UdpService::update] received msg: {}", buf);
    //     }
    // }
}


void idk::UdpService::shutdown(idk::IEngine*)
{
    VLOG_INFO("[UdpService::shutdown]");
    NET_Quit();
}

