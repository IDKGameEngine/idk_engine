#include "idk/platform/VideoManager.hpp"
#include "idk/platform/PlatformContext.hpp"

#include "libidk/Assert.hpp"
#include "libidk/log.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>


idk::VideoManager::VideoManager(PlatformContext &ctx, const char *title, int w, int h)
:   IPlatformFeature(ctx),
    mTitle(title),
    mWin(nullptr),
    mWidth(w),
    mHeight(h)
{
    if (false == SDL_Init(SDL_INIT_VIDEO))
    {
        VLOG_FATAL("{}", SDL_GetError());
    }

    if (!(mWin = SDL_CreateWindow(title, mWidth, mHeight, SDL_WINDOW_VULKAN)))
    {
        VLOG_FATAL("SDL_CreateWindow: {}", SDL_GetError());
    }
}


idk::VideoManager::~VideoManager()
{
    if (mWin != nullptr)
    {
        SDL_DestroyWindow((SDL_Window*)mWin);
        mWin = nullptr;
    }
}


void idk::VideoManager::onUpdate(idk::ServiceManager*)
{

}

void idk::VideoManager::setWindowFullscreen(bool fullscreen)
{
    SDL_SetWindowFullscreen((SDL_Window*)mWin, fullscreen);
}

void idk::VideoManager::setWindowVisibility(bool visible)
{
    if (mWin == nullptr)
    {
        return;
    }
    if (visible)
    {
        SDL_ShowWindow((SDL_Window*)mWin);
    }
    else
    {
        SDL_HideWindow((SDL_Window*)mWin);
    }
}

void idk::VideoManager::setWindowResolution(int w, int h)
{
    if (mWin == nullptr)
    {
        mWidth = w;
        mHeight = h;
        return;
    }

    SDL_SetWindowSize((SDL_Window*)mWin, w, h);
    mWidth = w;
    mHeight = h;
}

void idk::VideoManager::setRenderResolution(int w, int h)
{
    mWidth = w;
    mHeight = h;

    if (mWin != nullptr)
    {
        SDL_SetWindowSize((SDL_Window*)mWin, w, h);
    }
}
