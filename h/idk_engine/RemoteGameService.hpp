#pragma once

#include "idk/core/engine.hpp"
#include "RemoteGameClient.hpp"
#include "RemoteGameServer.hpp"

namespace idk
{
    class RemoteGameService: public idk::core::Service
    {
    public:
        RemoteGameService();
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;

    private:
        // idk::RaiiFunc<void()> mRaii;

    };

}
