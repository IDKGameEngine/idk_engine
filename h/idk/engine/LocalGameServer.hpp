#pragma once

#include "idk/core/log.hpp"
#include "idk/core/InplaceList.hpp"
#include "idk/core/types.hpp"

#include "idk/engine/UdpRxer.hpp"
#include "idk/engine/RemoteGameClient.hpp"

// #include "idk/engine/UdpRxerBase.hpp"
// #include "idk/config/udp/HandshakeAddress.hpp"

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

