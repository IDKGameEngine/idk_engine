#pragma once

#include "idk/core/Engine.hpp"
#include "idk/core/periodic_timer.hpp"
#include "config/Network.hpp"
#include "UdpRxTxer.hpp"

namespace idk::engine
{
    class GameClient
    {
    public:
        GameClient(uint16_t hostport);
        void update(idk::IEngine*);

    private:
        static void udpListenFunc(GameClient*);

        UdpRxTxer mRxTxer;
        PeriodicTimer mRttTimer;

    };

}
