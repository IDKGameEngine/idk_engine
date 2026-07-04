// #include "idk/UdpServerService.hpp"
// #include "idk/core/log.hpp"
// #include "idk/core/platform.hpp"
// #include "idk/core/stdmem.hpp"

// #include <SDL3/SDL.h>
// #include <SDL3_net/SDL_net.h>



// static void UdpServerServiceRaiiFunc()
// {
//     if (!NET_Init())
//     {
//         VLOG_FATAL("{}", SDL_GetError());
//     }
// }

// idk::UdpServerService::UdpServerService()
// :   Service("UdpServerService", idk_typeid<UdpServerService>()),
//     mRaii(UdpServerServiceRaiiFunc)
// {
    
// }

// void idk::UdpServerService::update(idk::IEngine*)
// {

// }

// void idk::UdpServerService::shutdown(idk::IEngine*)
// {
//     VLOG_INFO("[UdpServerService::shutdown]");
//     NET_Quit();
// }











// static void UdpClientServiceRaiiFunc()
// {
//     if (!NET_Init())
//     {
//         VLOG_FATAL("{}", SDL_GetError());
//     }
// }

// idk::UdpClientService::UdpClientService()
// :   Service("UdpClientService", idk_typeid<UdpClientService>()),
//     mRaii(UdpClientServiceRaiiFunc),
//     mTimeSyncRxer(),
//     mTimeSyncTxer()
// {

// }

// void idk::UdpClientService::update(idk::IEngine*)
// {
//     config::udp::TimeSyncData timeSyncData;

//     if (mTimeSyncRxer.recvmsg(timeSyncData))
//     {
//         uint64_t clientSendTime = timeSyncData.clientSendTime;
//         VLOG_INFO("TimeSyncData::clientSendTime: {}", clientSendTime);
//         timeSyncData.serverSendTime = idk::platform::GetSysTimeMs();
//         mTimeSyncTxer.sendmsg(timeSyncData);
//     }
// }

// void idk::UdpClientService::shutdown(idk::IEngine*)
// {
//     VLOG_INFO("[UdpClientService::shutdown]");
//     NET_Quit();
// }

