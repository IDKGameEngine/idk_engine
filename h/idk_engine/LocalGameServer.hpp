#pragma once

#include "idk/core/log.hpp"
#include "idk/core/InplaceList.hpp"
#include "idk/core/types.hpp"

#include "idk_engine/UdpRxer.hpp"
#include "idk_engine/RemoteGameClient.hpp"

// #include "idk_engine/UdpRxerBase.hpp"
// #include "idk_engine/config/udp/HandshakeAddress.hpp"

namespace idk::engine
{
    class LocalGameServer: private idk::NonMobile
    {
    public:
        static constexpr size_t MAX_CLIENTS = 32;
        LocalGameServer();
        ~LocalGameServer();
        void update();

    private:
        idk::InplaceList<RemoteGameClient, MAX_CLIENTS> mClients;

    };
}

