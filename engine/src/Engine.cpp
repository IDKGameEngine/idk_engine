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

    setFixedUpdateRate(30.0);

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


void idk::Engine::setFixedUpdateRate(double rateHz)
{
    mFixedAccumulator.setRateHz(rateHz);
    mApi.mFixedDeltaTimeSec = mFixedAccumulator.getStepSec();
}


void idk::Engine::startApplication(idk::Service *app)
{
    addService(app);
    dispatchInit(mApi);

    while (!mShouldQuit.load())
    {
        constexpr double MaxDeltaTimeSec = 0.25;
        mApi.mDeltaTimeSec = std::min(mClock.tick(), MaxDeltaTimeSec);

        processEvents();
        dispatchUpdate(mApi);

        mFixedAccumulator.addDeltaTimeSec(mApi.getDeltaTimeSec());
        while (mFixedAccumulator.step())
        {
            dispatchFixedUpdate(mApi);
        }

        dispatchPreRender(mApi);
        dispatchMidRender(mApi);
        dispatchPostRender(mApi);
    }

    dispatchShutdown(mApi);
}


void idk::Engine::processEvents()
{
    EngineEvent e;
    while (mApi.mEventQueue.pop(e))
    {
        processEvent(e);
    }
}


void idk::Engine::processEvent(const EngineEvent &e)
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
