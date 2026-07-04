#pragma once

#include "idk/core/Engine.hpp"
#include "idk/core/periodic_timer.hpp"
#include "idk/core/InplaceList.hpp"

#include "config/Network.hpp"
#include "UdpRxTxer.hpp"

#include <atomic>


namespace idk::engine
{
    class GameServer;
}


class idk::engine::GameServer: public idk::NonMobile
{
public:
    GameServer();
    void update(idk::IEngine*);

private:
    static void udpListenFunc(GameServer*);

    config::network::UdpRttPacket mRttQueue[64];
    std::atomic<int64_t> mRttRd, mRttWt;

    config::network::UdpInputPacket mCtrlQueue[64];
    std::atomic<int64_t> mCtrlRd, mCtrlWt;

};

