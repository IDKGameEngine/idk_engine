#pragma once

#include "idk/core/Engine.hpp"
#include "idk/core/Service.hpp"
#include "idk/core/InplaceList.hpp"
#include "idk_engine/EntityManager.hpp"
#include "idk/message/EventChannel.hpp"

namespace idk::engine
{
    struct EntityEvent
    {
        uint8_t tag;
        uint8_t data[256 - sizeof(tag)];
    };

    class WorldService: public idk::core::Service
    {
    public:
        WorldService();
        ~WorldService();
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;
    
    private:
        idk::engine::EntityManager mEntityManager;
        idk::EventChannel<EntityEvent> mEntityEvents;
    };

}


