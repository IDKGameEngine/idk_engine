#pragma once

#include "idk/core/engine.hpp"
#include "idk/core/periodic_timer.hpp"
#include "idk/core/InplaceList.hpp"

#include "config/Network.hpp"
#include "UdpRxTxer.hpp"

#include <atomic>


namespace idk::engine
{
    /**
     * Representation of local game instance
     */
    class LocalGameClient;

    /**
     * Responsible for LocalGameClient and RemoteGameServer
     */
    class GameServerService;

}


class idk::engine::GameServerService: public idk::core::Service
{
public:
    GameServerService();
    virtual void update(idk::IEngine*) final;
    virtual void shutdown(idk::IEngine*) final;

private:
    static void udpListenFunc(GameServerService*);

    idk::RaiiFunc<void()> mRaii;

    config::network::UdpRttPacket mRttQueue[64];
    std::atomic<int64_t> mRttRd, mRttWt;

    config::network::UdpInputPacket mCtrlQueue[64];
    std::atomic<int64_t> mCtrlRd, mCtrlWt;

};

