#pragma once

#include "idk/core/engine.hpp"
#include "LocalGameClient.hpp"
#include "LocalGameServer.hpp"

namespace idk
{
    class LocalGameService: public idk::core::Service
    {
    public:
        LocalGameService();
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;

    private:
        idk::RaiiFunc<void()> mRaii;
        idk::engine::LocalGameClient mLocalClient;

    };

}
