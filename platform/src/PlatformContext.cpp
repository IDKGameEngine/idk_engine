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
{
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

    addService<idk::AudioManager>(*this);
    addService<idk::EventManager>(*this);
    addService<idk::FilesystemManager>(*this);
    addService<idk::InputManager>(*this);
    addService<idk::VideoManager>(*this, cfg.windowTitle, cfg.windowWidth, cfg.windowHeight);

    this->initServices();
}


idk::PlatformContext::~PlatformContext()
{
    SDL_Quit();
}


// bool idk::PlatformContext::running()
// {
//     return mRunning.load(std::memory_order_acquire);
// }


// void idk::PlatformContext::shutdown()
// {
//     mShutdown.store(true, std::memory_order_release);
// }


// void idk::PlatformContext::update()
// {
//     this->updateServices();

//     if (mShutdown.load(std::memory_order_acquire))
//     {
//         this->shutdownServices();
//         mRunning.store(false, std::memory_order_release);
//     }
// }


void idk::PlatformContext::onInit(ServiceManager*)
{
    initServices();
}

void idk::PlatformContext::onUpdate(ServiceManager*)
{
    updateServices();
}

void idk::PlatformContext::onShutdown(ServiceManager*)
{
    shutdownServices();
}

void idk::PlatformContext::onEvent(ServiceManager*, const void*)
{

}

