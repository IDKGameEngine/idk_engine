#include "idk/gfx/GfxService.hpp"
#include "vk/GraphicsDevice.hpp"
#include "libidk/New.hpp"


idk::gfx::GfxService::GfxService(idk::PlatformContext &plat)
:   mPlat(plat),
    mGfxDevice(nullptr)
// :   mGfxDevice(idk::New<GraphicsDevice>())
{

}

idk::gfx::GfxService::~GfxService()
{

}

void idk::gfx::GfxService::onInit(idk::ServiceManager*)
{
    mGfxDevice = idk::New<GraphicsDevice>();
}

void idk::gfx::GfxService::onUpdate(idk::ServiceManager*)
{
    
}

void idk::gfx::GfxService::onShutdown(idk::ServiceManager*)
{
    
}

void idk::gfx::GfxService::onEvent(ServiceManager*, const void*)
{

}
