#pragma once

#include "IPlatformContext.hpp"


namespace idk
{
    struct PlatformConfig
    {
        const char* windowTitle;
        int windowWidth;
        int windowHeight;
    };

    class PlatformContext: public idk::IPlatformContext
    {
    public:
        PlatformContext(const PlatformConfig &cfg);
        ~PlatformContext();
    };

}
