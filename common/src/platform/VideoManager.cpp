#include "idk/platform/VideoManager.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>


idk::VideoManager::VideoManager(const char *title, int w, int h)
:   mTitle(title), mWin(nullptr), mWidth(w), mHeight(h) {  };


void idk::VideoManager::onInit(idk::EngineAPI&)
{
    if (false == SDL_Init(SDL_INIT_VIDEO))
    {
        VLOG_FATAL("{}", SDL_GetError());
    }

    SDL_WindowFlags windowFlags = 0;
    // #ifdef IDK_OPENGL
        windowFlags |= SDL_WINDOW_OPENGL;
    // #else
        // windowFlags |= SDL_WINDOW_VULKAN;
    // #endif

    if (!(mWin = SDL_CreateWindow(mTitle, mWidth, mHeight, windowFlags)))
    {
        VLOG_FATAL("SDL_CreateWindow: {}", SDL_GetError());
    }
}


void idk::VideoManager::onShutdown(idk::EngineAPI&)
{
    if (mWin != nullptr)
    {
        SDL_DestroyWindow((SDL_Window*)mWin);
        mWin = nullptr;
    }
}


void idk::VideoManager::onUpdate(idk::EngineAPI&)
{

}


void idk::VideoManager::onEvent(idk::EngineAPI&, const void*)
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
