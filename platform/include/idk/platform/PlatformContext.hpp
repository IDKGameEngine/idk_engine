#pragma once

#include "libidk/Service.hpp"
// #include "libidk/Types.hpp"
// #include "libidk/Assert.hpp"
// #include "libidk/dsa/List.hpp"
// #include "libidk/New.hpp"
// #include <atomic>


namespace idk
{
    struct PlatformConfig
    {
        const char* windowTitle;
        int windowWidth;
        int windowHeight;
    };

    class PlatformContext: public idk::Service, public idk::ServiceManager
    {
    public:
        PlatformContext(const PlatformConfig &cfg);
        ~PlatformContext();

        // bool running();
        // void shutdown();
        // void update();
        virtual void onInit(ServiceManager*) final;
        virtual void onUpdate(ServiceManager*) final;
        virtual void onShutdown(ServiceManager*) final;
        virtual void onEvent(ServiceManager*, const void*) final;

    private:
        // std::atomic<bool> mRunning {true};
        // std::atomic<bool> mShutdown {false};

    };

}
