#pragma once

#include "libidk/Service.hpp"
#include "idk/platform/PlatformContext.hpp"


namespace idk::gfx
{
    class RenderEngine;

    class GfxService: public idk::Service
    {
    public:
        GfxService(idk::PlatformContext&);
        ~GfxService();
        virtual void onInit(ServiceManager*) final;
        virtual void onUpdate(ServiceManager*) final;
        virtual void onShutdown(ServiceManager*) final;
        virtual void onEvent(ServiceManager*, const void*) final;

    private:
        idk::PlatformContext   &mPlat;
        idk::gfx::RenderEngine *mRenderEngine;

    };
}
