#pragma once

#include "idk/core/engine.hpp"
#include "idk/core/periodic_timer.hpp"

#include "config/udp/ClientHealthAddress.hpp"
#include "config/udp/ClientInputAddress.hpp"

#include "RemoteGameServer.hpp"
#include "UdpRxer.hpp"
#include "UdpTxer.hpp"


namespace idk::engine
{
    /**
     * Representation of local game instance
     */
    class LocalGameClient;

    /**
     * Responsible for LocalGameClient and RemoteGameServer
     */
    class LocalGameService;

}


class idk::engine::LocalGameClient: private idk::NonMobile
{
public:
    friend class idk::engine::LocalGameService;
    using HealthData = config::udp::ClientHealthData;
    using InputData = config::udp::ClientInputData;

private:
    uint64_t mPingMs;

    idk::PeriodicTimer mHealthTimer;
    idk::PeriodicTimer mInputTimer;
    HealthData mHealthData;
    InputData mInputData;
    UdpTxer2<config::udp::ClientHealthAddress> mHealthTx;
    UdpTxer2<config::udp::ClientInputAddress> mInputTx;
    UdpRxer2<config::udp::ClientHealthAddress> mHealthRx;
    UdpRxer2<config::udp::ClientInputAddress> mInputRx;

    LocalGameClient();
    void update();

};


class idk::engine::LocalGameService: public idk::core::Service
{
public:
    LocalGameService();
    virtual void update(idk::IEngine*) final;
    virtual void shutdown(idk::IEngine*) final;

private:
    idk::RaiiFunc<void()> mRaii;
    idk::engine::LocalGameClient mLocalClient;

};

