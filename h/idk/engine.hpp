#pragma once

#include "idk/core/engine.hpp"
#include "idk/core/periodic_timer.hpp"

#include <atomic>
#include <barrier>
#include <initializer_list>
#include <mutex>
#include <thread>
#include <vector>


namespace idk
{
    class Engine: public idk::IEngine
    {
    public:
        Engine(std::initializer_list<core::Service*> services);
        ~Engine();

        virtual bool running() final;
        virtual void shutdown() final;
        virtual void update() final;

    private:
        std::atomic<bool> running_;

    };

}

