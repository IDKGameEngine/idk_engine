#pragma once

#include "idk/core/types.hpp"


namespace idk::engine
{
    struct Entity
    {
        uint32_t idx;
        uint32_t gen;
    };

    class EntityManager: public idk::NonMobile
    {
    private:
        const size_t mMaxEntities;
        uint32_t mNextIdx;
        uint32_t mFreeListSize;
        uint32_t *mFreeList;
        uint32_t *mGen;
        bool     *mAlive;

    public:
        EntityManager(size_t maxEntities);
        // void update();
        Entity createEntity();
        void destroyEntity(const Entity &E);
        bool isAlive(const Entity &E);
    };

}
