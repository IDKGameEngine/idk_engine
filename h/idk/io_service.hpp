#pragma once

#include "idk/core/double_buffer.hpp"
#include "idk/core/engine.hpp"
#include "idk/core/file.hpp"

#include "idk/engine/io_interface.hpp"


namespace idk
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

