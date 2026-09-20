#pragma once

#include "idk/engine/IApplication.hpp"
#include "idk/platform/IPlatformContext.hpp"
#include "libidk/Service.hpp"

namespace idk
{
    class Engine;
}


class idk::Engine: public idk::ServiceManager
{
public:
    Engine(idk::IPlatformContext *plat);
    void run(idk::IApplication *app);
    IPlatformContext *getPlatformContext();

private:
    idk::IPlatformContext *mPlat;

    bool should_quit();

};
