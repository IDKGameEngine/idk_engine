#pragma once

#include "idk_engine/NetService.hpp"
#include "idk/core/Engine.hpp"
#include "UdpRxTxer.hpp"

namespace idk::engine
{
    class GameClient
    {
    public:
        GameClient(uint16_t hostport);
        void update(idk::IEngine*);

    private:
        UdpRxTxer mRxTxer;
        PeriodicTimer mRttTimer;

        static void udpListenFunc(GameClient*);
    };

}
