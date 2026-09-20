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

void idk::gfx::GfxService::init(idk::ServiceManager *sm)
{
    (void)sm;
}

void idk::gfx::GfxService::update(idk::ServiceManager *sm)
{
    (void)sm;
}
