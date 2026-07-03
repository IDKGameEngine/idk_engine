#include "idk/UdpService.hpp"
#include "idk/core/log.hpp"


static void UdpServiceRaiiFunc()
{
    if (!NET_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}


idk::UdpService::UdpService()
:   Service("UdpService", idk_typeid<UdpService>()),
    mRaii(UdpServiceRaiiFunc)
{
    mRxers.push(UdpRxer(mCfg["PORT_IO_CTRL"].getValueU16()));
    mRxers.push(UdpRxer(mCfg["PORT_UDP_CTRL"].getValueU16()));
    mRxers.push(UdpRxer(mCfg["PORT_ENGINE_CTRL"].getValueU16()));

}


void idk::UdpService::startup(idk::IEngine*)
{
    VLOG_INFO("[UdpService::startup]");

    if (!NET_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }

}


void idk::UdpService::update(idk::IEngine*)
{
    for (UdpRxer &rxer: mRxers)
    {
        rxer.update();
    }

    // NET_Datagram *dgram = NULL;
    // while (NET_ReceiveDatagram(mSocketEngineCtrl, &dgram))
    // {
    //     if (!dgram)
    //     {
    //         break;
    //     }

    //     VLOG_INFO("[UdpService::update] received {}B: {}", dgram->buflen, (const char*)(dgram->buf));

    //     // Can inspect dgram->addr and dgram->port for sender information
    //     // dgram->addr
    //     // dgram->port

    //     // Always free datagram when finished
    //     NET_DestroyDatagram(dgram);
    // }
}


void idk::UdpService::shutdown(idk::IEngine*)
{
    VLOG_INFO("[UdpService::shutdown]");
    
    // NET_DestroyDatagramSocket(mSocketEngineCtrl);
    NET_Quit();
}

