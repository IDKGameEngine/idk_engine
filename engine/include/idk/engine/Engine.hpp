#pragma once

#include "idk/engine/IApplication.hpp"
#include "idk/platform/PlatformContext.hpp"
#include "libidk/Service.hpp"

namespace idk
{
    class Engine;
}


class idk::Engine: public idk::ServiceManager
{
public:
    Engine();
    void run(idk::IApplication *app);
    IPlatformContext *getPlatformContext();

private:
    PlatformContext mPlat;

    bool should_quit();

};
