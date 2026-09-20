#pragma once

#include "libidk/Service.hpp"


namespace idk
{
    class PlatformContext;

    class IPlatformFeature: public idk::Service
    {
    protected:
        idk::PlatformContext &mCtx;

    public:
        IPlatformFeature(idk::PlatformContext &ctx): mCtx(ctx) {  };
        virtual ~IPlatformFeature() = default;
        virtual void onInit(idk::ServiceManager*) override {  };
        virtual void onUpdate(idk::ServiceManager*) override {  };
        virtual void onShutdown(idk::ServiceManager*) override {  };
        virtual void onEvent(idk::ServiceManager*, const void*) override {  };
    };
}
