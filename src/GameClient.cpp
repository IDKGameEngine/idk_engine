#include "idk_engine/GameClient.hpp"

#include "idk/core/Platform.hpp"
#include "idk/core/stdmem.hpp"

#include <SDL3_net/SDL_net.h>
#include <thread>


void idk::engine::GameClient::udpListenFunc(GameClient *srv)
{
    UdpRxTxer &rttRxTx = srv->mRxTxer;

    while (true)
    {
        static NetProtocol::UdpPacket udpData;

        if (NET_Datagram *dgram = rttRxTx.beginRecvMsg(&udpData, sizeof(udpData)))
        {
            switch (udpData.tag)
            {
                using namespace NetProtocol;

                case UdpTag::RoundTripTime:
                {
                    uint64_t clientTime = udpData.as_RoundTripTime.clientSendTime;
                    // uint64_t serverTime = udpData.as_RoundTripTime.serverSendTime;
                    uint64_t rdTripTime = Platform::getSysTimeNs() - clientTime;
                    VLOG_INFO("[GameClient] Ping: {} ns", rdTripTime);
                    break;
                }

                default:
                    VLOG_WARN("[GameClient] Recv Invalid (tag={})", static_cast<uint8_t>(udpData.tag));
                    break;
            }

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
    static NetProtocol::UdpPacket udpData;

    if (mRttTimer.expired())
    {
        mRttTimer.reset();

        udpData = {
            .tag = NetProtocol::UdpTag::RoundTripTime,
            .as_RoundTripTime = {
                .clientSendTime = Platform::getSysTimeNs(),
                .serverSendTime = 0
            }
        };

        mRxTxer.sendMsg(&udpData, sizeof(udpData));
    }
}

