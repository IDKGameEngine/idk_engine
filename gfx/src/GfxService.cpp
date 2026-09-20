#include "idk/gfx/GfxService.hpp"
#include "idk/platform/VideoManager.hpp"
#include "libidk/New.hpp"
#include "vk/VulkanContext.hpp"


idk::gfx::GfxService::GfxService(idk::PlatformContext &plat)
:   mPlat(plat),
    mVulkanContext(nullptr)
// :   mGfxDevice(idk::New<GraphicsDevice>())
{

}

idk::gfx::GfxService::~GfxService()
{

}

void idk::gfx::GfxService::onInit(idk::ServiceManager*)
{
    auto *video = mPlat.getService<idk::VideoManager>();
    void *win   = video->getWindowHandle();
    mVulkanContext = idk::New<idk::gfx::VulkanContext>(win);
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
