#pragma once

#include "idk_engine/Engine.hpp"
#include "UdpRxTxer.hpp"


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
    static void udpListenFunc(GameServer*);

};

