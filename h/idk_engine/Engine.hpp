#pragma once

#include "idk/core/Engine.hpp"
#include "idk/core/Service.hpp"

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
        std::vector<core::Service*> srvs_;

        virtual core::Service *_getService(idk::IdType id) final;

    };

}

