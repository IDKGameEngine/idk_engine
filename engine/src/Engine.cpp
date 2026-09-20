#include "idk/engine/Engine.hpp"
#include "idk/engine-message/EventQueue.hpp"
#include "idk/gfx/GfxService.hpp"


idk::Engine::Engine()
:   ServiceManager(),
    mShouldQuit(false)
{
    addService<idk::PlatformContext>(PlatformConfig{"GameWindow", 1280, 720});
    addService<idk::gfx::GfxService>(*getService<PlatformContext>());
}


void idk::Engine::run(idk::IApplication *app)
{
    // mPlat.initServices();
    initServices();
    app->onInit(*this);

    while (mShouldQuit.load() == false)
    {
        // mPlat.update();
        updateServices();
        app->onUpdate(*this);

        EngineEvent e;
        while (EngineEvent::gEngineEventQueue.pop(e))
        {
            process_engine_event(e);
        }
    }

    app->onShutdown(*this);
    shutdownServices();
}


// idk::PlatformContext *idk::Engine::getPlatform()
// {
//     return &mPlat;
// }


void idk::Engine::process_engine_event(const EngineEvent &e)
{
    switch (e.type)
    {
        case EngineEvent::PAUSE:
            break;
        case EngineEvent::RESUME:
            break;
        case EngineEvent::SHUTDOWN:
            mShouldQuit.store(true);
            break;
    }
}
