#pragma once

#include "idk/core/engine.hpp"
#include "idk/core/periodic_timer.hpp"
#include "UdpRxTxer.hpp"
#include "config/Network.hpp"

namespace idk::engine
{
    class GameClientService: public idk::core::Service
    {
    public:
        GameClientService();
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;

    private:
        static void udpListenFunc(GameClientService*);

        RaiiFunc<void()> mRaii;
        UdpTxer mRttTx;
        PeriodicTimer mRttTimer;

    };

}
