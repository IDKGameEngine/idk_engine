#pragma once

#include "libidk/Types.hpp"

namespace idk
{
    class Engine;

    class IApplication: public idk::Immobile
    {
    public:
        virtual ~IApplication() = default;
        virtual void onInit(Engine&) = 0;
        virtual void onUpdate(Engine&) = 0;
        virtual void onShutdown(Engine&) = 0;
    };

}
