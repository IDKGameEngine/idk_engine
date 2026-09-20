#include "idk/engine/Engine.hpp"


idk::Engine::Engine(idk::IPlatformContext *plat)
:   ServiceManager(),
    mPlat(plat)
{

}


void idk::Engine::start()
{
    for (idk::Service *srv: mServices)
    {
        srv->init(this);
    }

    while (mPlat->running())
    {
        mPlat->update();

        for (idk::Service *srv: mServices)
        {
            srv->update(this);
        }
    }
}

