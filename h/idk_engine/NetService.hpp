#pragma once

#include "libidk/Engine.hpp"
#include "libidk/Service.hpp"

namespace idk
{
    class NetService: public idk::core::Service
    {
    private:
        idk::platform::Platform &mPlat;

    public:
        NetService(idk::platform::Platform&);
        ~NetService();

        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;

    };
}
