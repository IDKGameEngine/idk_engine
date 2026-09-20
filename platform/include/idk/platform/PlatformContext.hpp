#pragma once

#include "libidk/Service.hpp"


namespace idk
{
    struct PlatformConfig
    {
        const char* windowTitle;
        int windowWidth;
        int windowHeight;
    };

    class PlatformContext: public idk::Service, public idk::ServiceManager
    {
    public:
        PlatformContext(const PlatformConfig &cfg);
        ~PlatformContext();

        virtual void onInit(ServiceManager*) final;
        virtual void onUpdate(ServiceManager*) final;
        virtual void onShutdown(ServiceManager*) final;
        virtual void onEvent(ServiceManager*, const void*) final;

    };

}
