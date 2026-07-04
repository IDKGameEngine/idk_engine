#pragma once

#include "idk_engine/NetProtocol.hpp"

#include "idk/core/Engine.hpp"
#include "idk/core/Service.hpp"

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
    const uint16_t mServerPort;
    GameClient *mGameClient;
    GameServer *mGameServer;
};

