#pragma once

#include "libidk/Service.hpp"


namespace idk::gfx
{
    class GfxService: public idk::Service
    {
    public:
        GfxService();
        ~GfxService();
        virtual void onInit(ServiceManager*) final;
        virtual void onUpdate(ServiceManager*) final;
        virtual void onShutdown(ServiceManager*) final;
        virtual void onEvent(ServiceManager*, const void*) final;

    private:
        // GraphicsDevice *mGfxDevice;

    };
}
