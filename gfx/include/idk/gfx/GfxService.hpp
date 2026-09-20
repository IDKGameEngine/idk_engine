#pragma once

#include "libidk/Service.hpp"


namespace idk::gfx
{
    class GfxService: public idk::Service
    {
    public:
        GfxService();
        ~GfxService();
        virtual void init(ServiceManager*) final;
        virtual void update(ServiceManager*) final;

    private:
        // GraphicsDevice *mGfxDevice;

    };
}
