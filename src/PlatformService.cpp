#include "idk_engine/PlatformService.hpp"
#include "idk/core/Platform.hpp"

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <filesystem>


static const char *PlatformServiceRaiiFunc()
{
    namespace fs = std::filesystem;

    SDL_SetAppMetadata("BitchAss", "v0.0.0", "com.mellic03.BitchAss");
    fs::current_path(fs::path(SDL_GetBasePath()) / fs::path(IDK_ASSETS_DIRNAME));

    if (false == SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        VLOG_FATAL("{}", SDL_GetError());
    }

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE))
    {
        VLOG_ERROR("{}", SDL_GetError());
    }

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4))
    {
        VLOG_ERROR("{}", SDL_GetError());
    }

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6))
    {
        VLOG_ERROR("{}", SDL_GetError());
    }

    if (!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4))
    {
        VLOG_ERROR("{}", SDL_GetError());
    }

    if (!SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1))
    {
        VLOG_ERROR("{}", SDL_GetError());
    }

    if (!SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  24))
    {
        VLOG_ERROR("{}", SDL_GetError());
    }

    if (!SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8))
    {
        VLOG_ERROR("{}", SDL_GetError());
    }

    return "PlatformService";
}

uint64_t idk::Platform::getSysTimeMs()
{
    return SDL_GetTicks();
}

uint64_t idk::Platform::getSysTimeNs()
{
    return SDL_GetTicksNS();
}



idk::engine::PlatformService::PlatformService(bool headless)
:   IPlatformService(PlatformServiceRaiiFunc(), idk_typeid<PlatformService>())
{
    SDL_WindowFlags flags = SDL_WINDOW_OPENGL;
    if (headless) { flags |= SDL_WINDOW_HIDDEN; }

    const char *title = mCfg["WINDOW_TITLE"].toStr();
    int width = mCfg["WINDOW_WIDTH"].toI32();
    int height = mCfg["WINDOW_HEIGHT"].toI32();

    if (!(mWin = SDL_CreateWindow(title, width, height, flags)))
    {
        VLOG_FATAL("SDL_CreateWindow: {}", SDL_GetError());
    }

    if (!(mGl = SDL_GL_CreateContext(mWin)))
    {
        VLOG_FATAL("SDL_GL_CreateContext: {}", SDL_GetError());
    }

    if (!SDL_GL_SetSwapInterval(1))
    {
        VLOG_WARN("SDL_GL_SetSwapInterval: {}", SDL_GetError());
    }

    int interval;
    if (!SDL_GL_GetSwapInterval(&interval))
    {
        VLOG_WARN("SDL_GL_GetSwapInterval: {}", SDL_GetError());
    }
    else
    {
        VLOG_INFO("SDL_GL_GetSwapInterval: interval={}", interval);
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        VLOG_FATAL("gladLoadGLLoader failure");
    }

    if (!SDL_GL_MakeCurrent(mWin, (SDL_GLContext)mGl))
    {
        VLOG_ERROR("SDL_GL_MakeCurrent: {}", SDL_GetError());
    }
    
    if (!SDL_Init(SDL_INIT_EVENTS))
    {
        VLOG_FATAL("[PlatformService] {}", SDL_GetError());
    }
}


idk::engine::PlatformService::~PlatformService()
{
    SDL_GL_DestroyContext((SDL_GLContext)mGl);
    SDL_DestroyWindow(mWin);
    SDL_Quit();
}


void idk::engine::PlatformService::update(idk::IEngine *E)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            E->shutdown();
        }

        if (e.type == SDL_EVENT_KEY_UP)
        {
            if (e.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                E->shutdown();
            }
        }
    }

    SDL_GL_MakeCurrent(mWin, (SDL_GLContext)mGl);
}


void idk::engine::PlatformService::shutdown(idk::IEngine*)
{

}



void idk::engine::PlatformService::getWindowSize(int &w, int &h)
{
    SDL_GetWindowSize(mWin, &w, &h);
}


void idk::engine::PlatformService::swapWindow()
{
    SDL_GL_SwapWindow(mWin);
}


