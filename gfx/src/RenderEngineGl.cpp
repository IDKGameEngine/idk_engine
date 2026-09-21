#include "idk/gfx/RenderEngine.hpp"
#include "idk/platform/VideoManager.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>


void idk::gfx::RenderEngine::onInit(EngineAPI &api)
{
    mWinCtx = api.mVideo->getWindowHandle();
    mGlCtx = SDL_GL_CreateContext((SDL_Window*)mWinCtx);

    if (mGlCtx == nullptr)
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
    if (!SDL_GL_MakeCurrent((SDL_Window*)mWinCtx, (SDL_GLContext)mGlCtx))
    {
        VLOG_ERROR("SDL_GL_MakeCurrent: {}", SDL_GetError());
    }
    // SDL_SetWindowRelativeMouseMode((SDL_Window*)mWinCtx, true);
    // SDL_ShowOpenFileDialog(file_dialog_callback, nullptr, (SDL_Window*)mWinCtx, NULL, 0, NULL, true);

    GLint mGlVersionMajor, mGlVersionMinor;
    gl::GetIntegerv(GL_MAJOR_VERSION, &mGlVersionMajor);
    gl::GetIntegerv(GL_MINOR_VERSION, &mGlVersionMinor);
    VLOG_INFO("Context supports OpenGL {}.{}", mGlVersionMajor, mGlVersionMinor);

    gl::CreateVertexArrays(1, &mDummyVao);
}


void idk::gfx::RenderEngine::onUpdate(EngineAPI&)
{
    SDL_GL_MakeCurrent((SDL_Window*)mWinCtx, (SDL_GLContext)mGlCtx);

    gl::ClearColor(0.25f, 0.25f, 0.50f, 1.0f);
    gl::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gl::BindVertexArray(mDummyVao);

    SDL_GL_SwapWindow((SDL_Window*)mWinCtx);
}


void idk::gfx::RenderEngine::onShutdown(EngineAPI&)
{
    gl::DeleteVertexArrays(1, &mDummyVao);
    SDL_GL_DestroyContext((SDL_GLContext)mGlCtx);
}


void idk::gfx::RenderEngine::onEvent(EngineAPI&, const void*)
{

}

