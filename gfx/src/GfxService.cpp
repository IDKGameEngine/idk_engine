#include "idk/gfx/GfxService.hpp"
#include "vk/GraphicsDevice.hpp"
#include "libidk/New.hpp"


idk::gfx::GfxService::GfxService()
// :   mGfxDevice(idk::New<GraphicsDevice>())
{

}

idk::gfx::GfxService::~GfxService()
{

}

void idk::gfx::GfxService::onInit(idk::ServiceManager *sm)
{
    (void)sm;
}

void idk::gfx::GfxService::onUpdate(idk::ServiceManager *sm)
{
    (void)sm;
}

void idk::gfx::GfxService::onShutdown(idk::ServiceManager *sm)
{
    (void)sm;
}

void idk::gfx::GfxService::onEvent(ServiceManager*, const void*)
{

}
