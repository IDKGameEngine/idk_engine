#include "idk/engine/Engine.hpp"

idk::Engine::Engine(idk::IPlatformContext *plat)
:   mRunning(true),
    mPlat(plat),
    mComponentIdx(0)
{

}

void idk::Engine::start()
{
    while (mPlat->running())
    {
        mPlat->update();

        for (size_t i=0; i<mComponentIdx; i++)
        {
            mComponents[i]->update();
        }
    }
}
