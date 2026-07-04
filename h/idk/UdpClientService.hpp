#pragma once

#include "idk/core/engine.hpp"
#include "idk/core/file.hpp"
#include "idk/core/InplaceList.hpp"
#include "idk/engine/UdpRxer.hpp"
#include "idk/engine/UdpTxer.hpp"
#include "idk/config/udp/TimeSyncAddress.hpp"

namespace idk
{
    class UdpClientService: public idk::core::Service
    {
    public:
        static constexpr size_t MAX_RXERS = 16;

        UdpClientService();
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;

    private:
        idk::RaiiFunc<void()> mRaii;
        idk::UdpRxer2<config::udp::ServerTimeSyncAddress> mTimeSyncRxer;
        idk::UdpTxer2<config::udp::ClientTimeSyncAddress> mTimeSyncTxer;
    };

}

