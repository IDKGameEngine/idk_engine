#include "idk/platform/PlatformContext.hpp"
#include "idk/platform/AudioManager.hpp"
#include "idk/platform/EventManager.hpp"
#include "idk/platform/FilesystemManager.hpp"
#include "idk/platform/InputManager.hpp"
#include "idk/platform/VideoManager.hpp"

#include <steam/steam_api.h>


idk::PlatformContextSdl3::PlatformContextSdl3(const PlatformConfig &cfg)
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

idk::PlatformContextSdl3::~PlatformContextSdl3()
{
    SDL_Quit();
}
