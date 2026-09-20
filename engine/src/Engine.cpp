#include "idk/engine/Engine.hpp"

idk::Engine::Engine(idk::IPlatformContext *plat)
:   mRunning(true),
    mPlat(plat)
{

}

void idk::Engine::start()
{
    for (EngineService *C: mComponents)
    {
        C->init(*this);
    }

    while (mPlat->running())
    {
        mPlat->update();

        for (EngineService *C: mComponents)
        {
            C->update(*this);
        }
        // for (size_t i=0; i<mComponentIdx; i++)
        // {
        //     mComponents[i]->update();
        // }
    }
}
