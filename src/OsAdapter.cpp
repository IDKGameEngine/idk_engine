#include "idk/OsAdapter.hpp"
#include "idk/core/log.hpp"
#include <filesystem>
#include <SDL3/SDL.h>


bool idk::OsAdapter::PreInit()
{
    VLOG_INFO("[idk::OsAdapter::PreInit]");

    if (false == SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_CAMERA))
        VLOG_FATAL("{}", SDL_GetError());

    std::filesystem::current_path(std::filesystem::path(SDL_GetBasePath()));

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


uint64_t idk::OsAdapter::GetSysTimeMs()
{
    return SDL_GetTicks();
}

uint64_t idk::OsAdapter::GetSysTimeNs()
{
    return SDL_GetTicksNS();
}

