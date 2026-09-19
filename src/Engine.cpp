#include "idk/engine/Engine.hpp"


idk::Engine::Engine(idk::IPlatformContext *plat)
:   mRunning(true),
    mPlat(plat),
    mServiceIdx(0)
{

}


void idk::Engine::start()
{
    while (mPlat->running())
    {
        mPlat->update();
    }
}

