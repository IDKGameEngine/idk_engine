#include "idk/core/platform.hpp"
#include "idk/core/log.hpp"
#include <filesystem>
#include <SDL3/SDL.h>


bool idk::platform::AppInit()
{
    namespace fs = std::filesystem;

    VLOG_INFO("[idk::platform::AppInit]");

    if (false == SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS))
        VLOG_FATAL("{}", SDL_GetError());

    fs::current_path(fs::path(SDL_GetBasePath()) / fs::path(IDK_ASSETS_DIRNAME));

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE))
        VLOG_ERROR("{}", SDL_GetError());

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4))
        VLOG_ERROR("{}", SDL_GetError());

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6))
        VLOG_ERROR("{}", SDL_GetError());

    if (!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4))
        VLOG_ERROR("{}", SDL_GetError());

    if (!SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1))
        VLOG_ERROR("{}", SDL_GetError());

    if (!SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  24))
        VLOG_ERROR("{}", SDL_GetError());

    if (!SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8))
        VLOG_ERROR("{}", SDL_GetError());

    return true;
}


uint64_t idk::platform::GetSysTimeMs()
{
    return SDL_GetTicks();
}

uint64_t idk::platform::GetSysTimeNs()
{
    return SDL_GetTicksNS();
}

