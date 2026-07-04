#pragma once

#include "idk/core/types.hpp"
#include <SDL3_net/SDL_net.h>

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
        NET_Address *mServerAddr;
        // char mHostname[MAX_HOSTNAME_LENGTH];

    };

}

