#include "idk_engine/GameServer.hpp"
#include "idk/core/stdmem.hpp"

#include <SDL3_net/SDL_net.h>
#include <thread>


void idk::engine::GameServer::udpListenFunc(GameServer*)
{
    UdpRxTxer udpRxTxer(config::network::SERVER_PORT);

    while (true)
    {
        static config::network::UdpRttPacket rttData;
        if (NET_Datagram *dgram = udpRxTxer.beginRecvMsg(&rttData, sizeof(rttData)))
        {
            rttData.serverSendTime = platform::GetSysTimeNs();
            udpRxTxer.replyMsg(dgram, &rttData, sizeof(rttData));
            VLOG_INFO("[GameServer] reply rtt: {}, {}", rttData.clientSendTime, rttData.serverSendTime);
            udpRxTxer.endRecvMsg(dgram);
        }
    }
}


idk::engine::GameServer::GameServer()
:   mRttRd{0},
    mRttWt{0},
    mCtrlRd{0},
    mCtrlWt{0}
{
    std::thread t(GameServer::udpListenFunc, this);
    t.detach();
}

void idk::engine::GameServer::update(idk::IEngine*)
{
    int64_t currRd = 0;

    currRd = mRttRd.load();
    if (currRd < mRttWt.load())
    {
        static config::network::UdpRttPacket rttData;
        idk_memcpy(&rttData, &mRttQueue[currRd], sizeof(rttData));
        VLOG_INFO("[GameServer::update] rttData: {}, {}", rttData.clientSendTime, rttData.serverSendTime);
        mRttRd.store((currRd + 1) % 64);
    }

    currRd = mCtrlRd.load();
    if (currRd < mCtrlWt.load())
    {
        // static config::network::UdpInputPacket ctrlData;

    }

}

