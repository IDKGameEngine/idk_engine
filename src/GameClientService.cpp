#include "idk_engine/GameClientService.hpp"

#include "idk/core/platform.hpp"
#include "idk/core/stdmem.hpp"

#include <SDL3_net/SDL_net.h>
#include <thread>


void idk::engine::GameClientService::udpListenFunc(GameClientService*)
{
    UdpRxer mRttRx;
    // UdpRxer mInputRx;

    while (true)
    {
        static config::network::UdpRttPacket rttData;
        if (NET_Datagram *dgram = mRttRx.beginRecvMsg(&rttData, sizeof(rttData)))
        {
            VLOG_INFO("[GameClientService] recv rtt: {}, {}", rttData.clientSendTime, rttData.serverSendTime);
            mRttRx.endRecvMsg(dgram);
        }

        // static config::network::UdpInputPacket ctrlData;
        // if (NET_Datagram *dgram = mInputRx.beginRecvMsg(&ctrlData, sizeof(ctrlData)))
        // {
        //     int64_t nextWt = (srv->mCtrlWt.load() + 1) % 64;
        //     if (nextWt == srv->mCtrlRd.load())
        //     {
        //         VLOG_WARN("[GameClientService::udpListenFunc] mCtrlQueue full\n");
        //     }
        //     else
        //     {
        //         srv->mCtrlQueue[nextWt] = ctrlData;
        //         srv->mCtrlWt.store(nextWt);
        //     }
        //     mHealthRx.endRecvMsg(dgram);
        // }
    }
}


static void GameServerServiceRaiiFunc()
{
    if (!NET_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}

idk::engine::GameClientService::GameClientService()
:   IDK_SERVICE_CTOR(GameClientService),
    mRaii(GameServerServiceRaiiFunc),
    mRttTx("127.0.0.1", 4001),
    mRttTimer(1)
{
    std::thread(GameClientService::udpListenFunc, this).detach();
}

void idk::engine::GameClientService::update(idk::IEngine*)
{
    static config::network::UdpRttPacket rttData;

    if (mRttTimer.expired())
    {
        mRttTimer.reset();

        rttData.clientSendTime = platform::GetSysTimeNs();
        mRttTx.sendmsg(&rttData, sizeof(rttData));
        VLOG_INFO("[GameClientService] send rtt: {}", rttData.clientSendTime);
    }
}

void idk::engine::GameClientService::shutdown(idk::IEngine*)
{

}

