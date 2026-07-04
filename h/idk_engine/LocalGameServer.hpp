#pragma once

#include <SDL3_net/SDL_net.h>

#include "RemoteGameClient.hpp"
#include "idk/core/InplaceList.hpp"


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

