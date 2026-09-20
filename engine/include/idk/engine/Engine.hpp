#pragma once

#include "libidk/Service.hpp"
#include "idk/platform/IPlatformContext.hpp"


namespace idk
{
    class Engine: public idk::ServiceManager
    {
    private:
        // idk::EngineContext     mContext;
        idk::IPlatformContext *mPlat;

    public:
        Engine(idk::IPlatformContext *plat);
        void start();

        IPlatformContext *getPlatformContext() { return mPlat; }
    };

}
