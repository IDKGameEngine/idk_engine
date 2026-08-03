#pragma once

#include "libidk/Engine.hpp"
#include "libidk/Service.hpp"

namespace idk
{
    class AudioService: public idk::core::Service
    {
    private:
        idk::Platform &mPlat;

    public:
        AudioService(idk::Platform&);
        ~AudioService();

        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;

    };
}
