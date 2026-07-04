#pragma once

#include "idk/core/Engine.hpp"
#include "idk/core/Service.hpp"
#include "idk/core/periodic_timer.hpp"
#include "idk/core/metric.hpp"
#include "idk/core/Allocator.hpp"
#include "idk/core/InplaceList.hpp"

#include "config/Network.hpp"
#include "UdpRxTxer.hpp"

#include <atomic>


namespace idk::engine
{
    class GameClient;
    class GameServer;
    class NetService;
}


class idk::engine::NetService: public idk::core::Service
{
public:
    NetService();
    ~NetService();

    virtual void update(idk::IEngine*) final;
    virtual void shutdown(idk::IEngine*) final;

    bool startGameClient();
    bool startGameServer();

    bool stopGameClient();
    bool stopGameServer();

private:
    GameClient *mGameClient;
    GameServer *mGameServer;
};

