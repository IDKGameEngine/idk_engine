#pragma once

#include "idk_engine/NetProtocol.hpp"

#include "idk_core/Engine.hpp"
#include "idk_core/Timer.hpp"
#include "idk_core/InplaceList.hpp"

#include "UdpRxTxer.hpp"

#include <atomic>


namespace idk::engine
{
    class GameServer;
}


class idk::engine::GameServer: public idk::NonMobile
{
public:
    GameServer(uint16_t port);
    void update(idk::IEngine*);

private:
    const uint16_t mPort;
    // NetProtocol::UdpRttPacket mRttQueue[64];
    // std::atomic<int64_t> mRttRd, mRttWt;

    // NetProtocol::UdpInputPacket mCtrlQueue[64];
    // std::atomic<int64_t> mCtrlRd, mCtrlWt;

    static void udpListenFunc(GameServer*);

};

