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

        virtual void onInit(EngineAPI&) final;
        virtual void onUpdate(EngineAPI&) final;
        virtual void onShutdown(EngineAPI&) final;
        virtual void onEvent(EngineAPI&, const void*) final;

    };

}
