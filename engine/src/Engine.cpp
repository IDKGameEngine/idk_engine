#include "idk/engine/Engine.hpp"
#include "idk/gfx/GfxService.hpp"


idk::Engine::Engine()
:   ServiceManager(),
    mPlat({"GameWindow", 1280, 720})
{
    this->addService<idk::gfx::GfxService>();
}


void idk::Engine::run(idk::IApplication *app)
{
    for (idk::Service *srv: mServices)
    {
        srv->init(this);
    }
    app->onInit(*this);

    while (!should_quit())
    {
        mPlat.update();
        for (idk::Service *srv: mServices)
        {
            srv->update(this);
        }
        app->onUpdate(*this);
    }

    app->onShutdown(*this);
}


idk::PlatformContext *idk::Engine::getPlatformContext()
{
    return &mPlat;
}


bool idk::Engine::should_quit()
{
    if (!mPlat.running())
    {
        return true;
    }

    return false;
}
