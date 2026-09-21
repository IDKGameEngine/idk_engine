#pragma once

#include "libidk/Service.hpp"

namespace idk
{
    class IApplication: public idk::Service
    {
    public:
        IApplication() = default;
        virtual ~IApplication() = default;

        virtual void onInit(EngineAPI&) = 0;
        virtual void onUpdate(EngineAPI&) = 0;
        virtual void onShutdown(EngineAPI&) = 0;
        virtual void onEvent(idk::EngineAPI&, const void*) = 0;
    };

}
