#pragma once

#include "libidk/Types.hpp"
#include "libidk/Service.hpp"
#include "libidk/platform/IPlatformContext.hpp"
#include <atomic>


namespace idk
{
    class Engine: public idk::Immobile
    {
    public:
        Engine(idk::IPlatformContext *plat);
        void start();

    private:
        static constexpr size_t MAX_SERVICES = 16;

        std::atomic_bool       mRunning;
        idk::IPlatformContext *mPlat;
        size_t                 mServiceIdx;
        idk::core::Service    *mServices[MAX_SERVICES];

    };

}

