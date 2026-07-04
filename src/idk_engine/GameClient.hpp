#pragma once

#include "idk_engine/NetProtocol.hpp"
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
        static void udpListenFunc(GameClient*);

        UdpRxTxer mRxTxer;
        PeriodicTimer mRttTimer;

    };

}
