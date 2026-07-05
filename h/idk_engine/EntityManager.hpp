#pragma once

#include "idk_engine/Engine.hpp"
#include "idk_core/types.hpp"

namespace idk::engine
{
    struct Entity
    {
        uint32_t idx;
        uint32_t gen;
    };

    struct EntityEvent
    {
        Entity entity;
        uint8_t data[256 - sizeof(Entity)];
    };


    class EntityManager: public idk::NonMobile
    {
    private:
        // idk::MessageTxer *mTxer;
        const size_t mMaxEntities;
        uint32_t mNextIdx;
        uint32_t mFreeListSize;
        uint32_t *mFreeList;
        uint32_t *mGen;
        bool     *mAlive;

        static void onEntityEvent(void *ctx, const EntityEvent&);

    public:
        EntityManager(size_t maxEntities);
        // void update();
        Entity createEntity();
        void destroyEntity(const Entity &E);
        bool isAlive(const Entity &E);
    };

}
