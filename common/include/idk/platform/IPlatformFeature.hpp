#pragma once

#include "libidk/Service.hpp"


namespace idk
{
    class EngineAPI;

    class IPlatformFeature: public idk::Service
    {
    public:
        IPlatformFeature(): Service() {  };
        virtual ~IPlatformFeature() = default;

        virtual void onInit(EngineAPI&) override {  };
        virtual void onUpdate(EngineAPI&) override {  };
        virtual void onShutdown(EngineAPI&) override {  };
        virtual void onEvent(EngineAPI&, const void*) override {  };
    };
}
