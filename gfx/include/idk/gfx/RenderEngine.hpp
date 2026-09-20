#pragma once

#include "libidk/gpu//gl.hpp"
#include "libidk/dsa/List.hpp"

namespace idk
{
    class PlatformContext;
}

namespace idk::gfx
{
    class RenderEngine;
}


class idk::gfx::RenderEngine
{
public:
    RenderEngine(PlatformContext&);
    ~RenderEngine();

    void update();
    void shutdown();

private:
    void    *mWinCtx;
    void    *mGlCtx;

    uint32_t mDummyVao;

};
