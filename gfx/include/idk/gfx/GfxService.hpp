#pragma once

#include "libidk/Service.hpp"
#include "idk/gfx/RenderEngine.hpp"
#include "idk/platform/PlatformContext.hpp"


namespace idk::gfx
{
    class GfxService: public idk::Service
    {
    private:
        idk::PlatformContext   &mPlat;
        idk::gfx::RenderEngine  mRenderEngine;

    public:
        GfxService(idk::PlatformContext&);
        ~GfxService();

        virtual void onInit(ServiceManager*) final;
        virtual void onUpdate(ServiceManager*) final;
        virtual void onShutdown(ServiceManager*) final;
        virtual void onEvent(ServiceManager*, const void*) final;

        idk::gfx::RenderEngine &getRenderEngine() { return mRenderEngine; }
    };
}
