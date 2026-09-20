#include "idk/gfx/GfxService.hpp"
#include "idk/platform/VideoManager.hpp"
#include "libidk/New.hpp"


idk::gfx::GfxService::GfxService(idk::PlatformContext &plat)
:   mPlat(plat),
    mRenderEngine(plat)
// :   mGfxDevice(idk::New<GraphicsDevice>())
{

}

idk::gfx::GfxService::~GfxService()
{

}

void idk::gfx::GfxService::onInit(idk::ServiceManager*)
{

}

void idk::gfx::GfxService::onUpdate(idk::ServiceManager*)
{
    mRenderEngine.update();
}

void idk::gfx::GfxService::onShutdown(idk::ServiceManager*)
{
    
}

void idk::gfx::GfxService::onEvent(ServiceManager*, const void*)
{

}
