#include "idk_engine/GameClient.hpp"
#include "idk_engine/NetProtocol.hpp"

#include "idk_core/Platform.hpp"
#include "idk_core/stdmem.hpp"

#include <SDL3_net/SDL_net.h>
#include <thread>


void idk::engine::GameClient::udpListenFunc(GameClient *gameClient)
{
    UdpRxTxer &rttRxTx = gameClient->mRxTxer;

    while (true)
    {
        static NetProtocol::Udpdata udpData;

        if (NET_Datagram *dgram = rttRxTx.beginRecvMsg(&udpData, sizeof(udpData)))
        {
            switch (udpData.tag)
            {
                using namespace NetProtocol;

                case UdpTag::RoundTripTime:
                {
                    static RoundTripTimeData rttData;
                    rttData.decode(udpData);
                    uint64_t roundTripTime = Platform::getSysTimeNs() - rttData.clientSendTime;
                    VLOG_INFO("[GameClient] Ping: {} ns", roundTripTime);
                    break;
                }

                case UdpTag::EntityEvent:
                {
                    static EntityEventData entEventData;
                    entEventData.decode(udpData);
                    VLOG_INFO("[GameClient] Recv EntityEvent");
                    // entEventTxer->sendMsg(&entEventData, sizeof(entEventData));
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
:   mRxTxer("127.0.0.1", hostport),
    mRttTimer(1)
{
    std::thread t(GameClient::udpListenFunc, this);
    t.detach();
}

void idk::engine::GameClient::update(idk::IEngine*)
{
    static NetProtocol::Udpdata udpData;

    if (mRttTimer.expired())
    {
        NetProtocol::RoundTripTimeData rttData = {
            .clientSendTime = Platform::getSysTimeNs(),
            .serverSendTime = 0
        };
        rttData.encode(udpData);
        mRxTxer.sendMsg(&udpData, sizeof(udpData));

        mRttTimer.reset();
    }
}

