#pragma once

#include <SDL3_net/SDL_net.h>
#include <atomic>

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
        bool isActive() const;

    private:
        static void udpListenFunc(LocalGameServer*);
        idk::InplaceList<RemoteGameClient, MAX_CLIENTS> mClients;
        std::atomic<bool> mActive;

    };
}

