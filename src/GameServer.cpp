#include "idk_engine/GameServer.hpp"
#include "idk/core/stdmem.hpp"

#include <SDL3_net/SDL_net.h>
#include <thread>


void idk::engine::GameServer::udpListenFunc(GameServer *server)
{
    UdpRxTxer udpRxTxer(server->mPort);

    while (true)
    {
        static NetProtocol::UdpPacket udpData;

        if (NET_Datagram *dgram = udpRxTxer.beginRecvMsg(&udpData, sizeof(udpData)))
        {
            switch (udpData.tag)
            {
                using namespace NetProtocol;

                case UdpTag::RoundTripTime:
                {
                    udpData.as_RoundTripTime.serverSendTime = Platform::getSysTimeNs();
                    uint64_t clientTime = udpData.as_RoundTripTime.clientSendTime;
                    uint64_t serverTime = udpData.as_RoundTripTime.serverSendTime;
                    udpRxTxer.replyMsg(dgram, &udpData, sizeof(udpData));
                    VLOG_INFO("[GameServer] Reply RTT: {}, {}", clientTime, serverTime);
                    break;
                }

                case UdpTag::UserCtrl:
                    VLOG_INFO("[GameServer] Ack UsrCtrl");
                    break;
                
                default:
                    VLOG_WARN("[GameServer] Recv Invalid (tag={})", static_cast<uint8_t>(udpData.tag));
                    break;
            }

            udpRxTxer.endRecvMsg(dgram);
        }
    }
}


idk::engine::GameServer::GameServer(uint16_t port)
:   mPort(port)
    // mRttRd{0},
    // mRttWt{0},
    // mCtrlRd{0},
    // mCtrlWt{0}
{
    std::thread t(GameServer::udpListenFunc, this);
    t.detach();
}

void idk::engine::GameServer::update(idk::IEngine*)
{
    // int64_t currRd = 0;

    // currRd = mRttRd.load();
    // if (currRd < mRttWt.load())
    // {
    //     NetProtocol::RoundTripTimeData rttData;
    //     idk_memcpy(&rttData, &mRttQueue[currRd], sizeof(rttData));
    //     VLOG_INFO("[GameServer::update] rttData: {}, {}", rttData.clientSendTime, rttData.serverSendTime);
    //     mRttRd.store((currRd + 1) % 64);
    // }

    // currRd = mCtrlRd.load();
    // if (currRd < mCtrlWt.load())
    // {
    //     // static config::network::UdpInputPacket ctrlData;

    // }

}

