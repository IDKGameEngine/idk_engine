#pragma once

#include "idk/core/log.hpp"
#include "idk/core/types.hpp"

#include "idk_engine/UdpRxerBase.hpp"


namespace idk::engine
{
    class RemoteGameServer: private idk::NonMobile
    {
    public:
        static constexpr size_t MAX_HOSTNAME_LENGTH = 256;
        RemoteGameServer(const char *hostname);
        ~RemoteGameServer();
        void update();

    private:
        void *mServerAddr;
        // char mHostname[MAX_HOSTNAME_LENGTH];

    };

}

