#include "idk/platform/PlatformContext.hpp"
#include "idk/platform/IPlatformFeature.hpp"
#include "idk/platform/AudioManager.hpp"
#include "idk/platform/EventManager.hpp"
#include "idk/platform/FilesystemManager.hpp"
#include "idk/platform/InputManager.hpp"
#include "idk/platform/VideoManager.hpp"

#include "libidk/log.hpp"

#include <steam/steam_api.h>


idk::PlatformContext::PlatformContext(const PlatformConfig &cfg)
:   mRunning{true},
    mShutdown{false}
{
    if (std::getenv("SteamEnv"))
    {
        SteamErrMsg errMsg = { 0 };
        if (SteamAPI_InitEx(&errMsg) == k_ESteamAPIInitResult_OK)
        {
            VLOG_FATAL("SteamAPI init success");
        }
        else
        {
            VLOG_FATAL("SteamAPI init failure: {}", errMsg);
        }
    }

    addFeature<idk::AudioManager>();
    addFeature<idk::EventManager>();
    addFeature<idk::FilesystemManager>();
    addFeature<idk::InputManager>();
    addFeature<idk::VideoManager>(cfg.windowTitle, cfg.windowWidth, cfg.windowHeight);
}


idk::PlatformContext::~PlatformContext()
{
    SDL_Quit();
}


bool idk::PlatformContext::running()
{
    return mRunning.load(std::memory_order_acquire);
}


void idk::PlatformContext::shutdown()
{
    mShutdown.store(true, std::memory_order_release);
}


void idk::PlatformContext::update()
{
    update_features();

    if (mShutdown.load(std::memory_order_acquire))
    {
        kill_features();
        mRunning.store(false, std::memory_order_release);
    }
}


void idk::PlatformContext::processEvent(const void *event)
{
    for (IPlatformFeature *feature: mFeatures)
    {
        feature->onEvent(*this, event);
    }
}


void idk::PlatformContext::update_features()
{
    for (IPlatformFeature *feature: mFeatures)
    {
        feature->onUpdate(*this);
    }
}


void idk::PlatformContext::kill_features()
{
    for (IPlatformFeature *feature: mFeatures)
    {
        feature->~IPlatformFeature();
    }
}
