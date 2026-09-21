#include "idk/engine/Engine.hpp"
#include "idk/gfx/RenderEngine.hpp"

#include <filesystem>
#include <steam/steam_api.h>


idk::Engine::Engine()
:   ServiceManager(),
    mShouldQuit(false),
    mApi(this, nullptr, nullptr, nullptr, nullptr)
{
    namespace fs = std::filesystem;

    if (std::getenv("SteamEnv"))
    {
        SteamErrMsg errMsg = { 0 };
        if (SteamAPI_InitEx(&errMsg) == k_ESteamAPIInitResult_OK)
        {
            VLOG_INFO("SteamAPI init success");
        }
        else
        {
            VLOG_FATAL("SteamAPI init failure: {}", errMsg);
        }
    }

    fs::current_path(fs::path(SDL_GetBasePath()) / fs::path(IDK_CONTENT_BASEPATH));

    addService<idk::AudioManager>();
    addService<idk::EventManager>();
    addService<idk::InputManager>();
    addService<idk::VideoManager>("GameWindow", 1280, 720);
    addService<idk::gfx::RenderEngine>();

    new (&mApi) EngineAPI(
        this,
        getService<AudioManager>(),
        getService<EventManager>(),
        getService<InputManager>(),
        getService<VideoManager>()
    );
}


void idk::Engine::run(idk::IApplication *app)
{
    initServices(mApi);
    app->onInit(mApi);

    while (mShouldQuit.load() == false)
    {
        updateServices(mApi);
        app->onUpdate(mApi);

        EngineEvent e;
        while (mApi.mEventQueue.pop(e))
        {
            process_engine_event(e);
        }
    }

    app->onShutdown(mApi);
    shutdownServices(mApi);
}


void idk::Engine::process_engine_event(const EngineEvent &e)
{
    if (e.type == EngineEvent::T_EngineCtl)
    {
        switch (e.subtype)
        {
            case EngineEvent::S_Pause:
                break;
            case EngineEvent::S_Resume:
                break;
            case EngineEvent::S_Shutdown:
                mShouldQuit.store(true);
                break;
        }
    }
}
