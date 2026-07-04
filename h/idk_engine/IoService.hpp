#pragma once

#include "idk/core/Engine.hpp"
#include "idk/core/Service.hpp"
#include "idk/core/file.hpp"

namespace idk::engine
{
    class IoService: public idk::core::Service
    {
    public:
        IoService();
        virtual void startup(idk::IEngine*) final;
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;

    };
}

