#include "idk_engine/GameServerService.hpp"
#include "idk/core/stdmem.hpp"

#include <SDL3_net/SDL_net.h>
#include <thread>


void idk::engine::GameServerService::udpListenFunc(GameServerService*)
{
    UdpRxer mRttRx(4001);
    // UdpRxer mInputRx(4002);

    while (true)
    {
        static config::network::UdpRttPacket rttData;
        if (NET_Datagram *dgram = mRttRx.beginRecvMsg(&rttData, sizeof(rttData)))
        {
            rttData.serverSendTime = platform::GetSysTimeNs();
            mRttRx.replyMsg(dgram, &rttData, sizeof(rttData));
            VLOG_INFO("[LocalGameServer:] reply rtt: {}, {}", rttData.clientSendTime, rttData.serverSendTime);
            mRttRx.endRecvMsg(dgram);
        }

        // static config::network::UdpInputPacket ctrlData;
        // if (NET_Datagram *dgram = mInputRx.beginRecvMsg(&ctrlData, sizeof(ctrlData)))
        // {
        //     int64_t nextWt = (srv->mCtrlWt.load() + 1) % 64;
        //     if (nextWt == srv->mCtrlRd.load())
        //     {
        //         VLOG_WARN("[GameServerService::udpListenFunc] mCtrlQueue full\n");
        //     }
        //     else
        //     {
        //         srv->mCtrlQueue[nextWt] = ctrlData;
        //         srv->mCtrlWt.store(nextWt);
        //     }
        //     mInputRx.endRecvMsg(dgram);
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

idk::engine::GameServerService::GameServerService()
:   IDK_SERVICE_CTOR(GameServerService),
    mRaii(GameServerServiceRaiiFunc),
    mRttRd{0},
    mRttWt{0},
    mCtrlRd{0},
    mCtrlWt{0}
{
    std::thread(GameServerService::udpListenFunc, this).detach();
}

void idk::engine::GameServerService::update(idk::IEngine*)
{
    int64_t currRd = 0;

    currRd = mRttRd.load();
    if (currRd < mRttWt.load())
    {
        static config::network::UdpRttPacket rttData;
        idk_memcpy(&rttData, &mRttQueue[currRd], sizeof(rttData));
        VLOG_INFO("[GameServerService::update] rttData: {}, {}", rttData.clientSendTime, rttData.serverSendTime);
        mRttRd.store((currRd + 1) % 64);
    }

    currRd = mCtrlRd.load();
    if (currRd < mCtrlWt.load())
    {
        // static config::network::UdpInputPacket ctrlData;

    }

}

void idk::engine::GameServerService::shutdown(idk::IEngine*)
{

}


