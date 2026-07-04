#include "idk_engine/GameClient.hpp"

#include "idk/core/platform.hpp"
#include "idk/core/stdmem.hpp"

#include <SDL3_net/SDL_net.h>
#include <thread>


void idk::engine::GameClient::udpListenFunc(GameClient *srv)
{
    UdpRxTxer &rttRxTx = srv->mRxTxer;

    while (true)
    {
        static config::network::UdpRttPacket rttData;
        if (NET_Datagram *dgram = rttRxTx.beginRecvMsg(&rttData, sizeof(rttData)))
        {
            VLOG_INFO("[GameClient] recv rtt: {}, {}", rttData.clientSendTime, rttData.serverSendTime);
            rttRxTx.endRecvMsg(dgram);
        }
    }
}


idk::engine::GameClient::GameClient(uint16_t hostport)
:   mRxTxer(),
    mRttTimer(1)
{
    mRxTxer.setRemote("127.0.0.1", hostport);
    std::thread t(GameClient::udpListenFunc, this);
    t.detach();
}

void idk::engine::GameClient::update(idk::IEngine*)
{
    static config::network::UdpRttPacket rttData;

    if (mRttTimer.expired())
    {
        mRttTimer.reset();

        rttData.clientSendTime = platform::GetSysTimeNs();
        mRxTxer.sendMsg(&rttData, sizeof(rttData));
        VLOG_INFO("[GameClient] send rtt: {}", rttData.clientSendTime);
    }
}

