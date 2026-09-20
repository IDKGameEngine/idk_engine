#pragma once

#include "libidk/Types.hpp"


namespace idk
{
    class PlatformContext;

    class IPlatformFeature: public idk::Immobile
    {
    public:
        IPlatformFeature() = default;
        virtual ~IPlatformFeature() = default;
        virtual void onInit(idk::PlatformContext&) {  };
        virtual void onUpdate(idk::PlatformContext&) {  };
        virtual void onShutdown(idk::PlatformContext&) {  };
        virtual void onEvent(idk::PlatformContext&, const void*) {  };
    };
}
