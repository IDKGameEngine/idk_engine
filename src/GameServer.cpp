#include "idk_engine/GameServer.hpp"
#include "idk_engine/NetProtocol.hpp"
#include "idk_core/stdmem.hpp"

#include <SDL3_net/SDL_net.h>
#include <thread>


void idk::engine::GameServer::udpListenFunc(GameServer *server)
{
    UdpRxTxer udpRxTxer(server->mPort);

    while (true)
    {
        static NetProtocol::Udpdata udpData;

        if (NET_Datagram *dgram = udpRxTxer.beginRecvMsg(&udpData, sizeof(udpData)))
        {
            switch (udpData.tag)
            {
                using namespace NetProtocol;

                case UdpTag::RoundTripTime:
                {
                    static NetProtocol::RoundTripTimeData rttData;
                    rttData.decode(udpData);
                    rttData.serverSendTime = Platform::getSysTimeNs();
                    rttData.encode(udpData);
                    udpRxTxer.replyMsg(dgram, &udpData, sizeof(udpData));
                    VLOG_INFO("[GameServer] Reply RTT: {}, {}", rttData.clientSendTime, rttData.serverSendTime);
                    break;
                }

                case UdpTag::UserCtrl:
                    static NetProtocol::UserCtrlData ctrlData;
                    ctrlData.decode(udpData);
                    VLOG_INFO("[GameServer] Ack UsrCtrl: {}", ctrlData.clientSendTime);
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

