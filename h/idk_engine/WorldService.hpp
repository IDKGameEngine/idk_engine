#pragma once

#include "idk_engine/Engine.hpp"
#include "idk_engine/EntityManager.hpp"
#include "idk/core/InplaceList.hpp"

namespace idk::engine
{
    class WorldService: public idk::core::Service
    {
    public:
        WorldService();
        ~WorldService();
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;
    
    private:
        idk::engine::EntityManager mEntityManager;

    };

}


