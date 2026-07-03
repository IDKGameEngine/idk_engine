#pragma once

#include "idk/core/engine.hpp"
#include "idk/core/file.hpp"
#include "idk/core/InplaceList.hpp"
#include "idk/engine/UdpRxer.hpp"

namespace idk
{
    class UdpService: public idk::core::Service
    {
    public:
        static constexpr size_t MAX_RXERS = 16;

        UdpService();
        virtual void startup(idk::IEngine*) final;
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;

    private:
        idk::InplaceList<idk::UdpRxer, MAX_RXERS> mRxers;

    };

}

