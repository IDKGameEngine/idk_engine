#pragma once

#include "idk/core/engine.hpp"

#include <atomic>
#include <initializer_list>

namespace idk
{
    class Engine: public idk::IEngine
    {
    public:
        Engine(std::initializer_list<core::Service*> services);
        virtual bool running() final;
        virtual void shutdown() final;
        virtual void update() final;

    private:
        std::atomic<bool> running_;

    };

}

