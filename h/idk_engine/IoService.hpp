#pragma once

#include "idk/core/engine.hpp"
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

