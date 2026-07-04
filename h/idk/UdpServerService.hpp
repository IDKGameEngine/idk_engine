#pragma once

#include "idk/core/engine.hpp"
#include "idk/core/file.hpp"
#include "idk/core/InplaceList.hpp"
#include "idk/engine/UdpRxer.hpp"
#include "idk/engine/UdpTxer.hpp"

namespace idk
{
    class UdpServerService: public idk::core::Service
    {
    public:
        static constexpr size_t MAX_RXERS = 16;

        UdpServerService();
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;

    private:
        idk::RaiiFunc<void()> mRaii;

    };

}

