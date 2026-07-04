#pragma once

#include "idk/core/Engine.hpp"
#include "idk/core/Service.hpp"
#include "idk/core/InplaceList.hpp"
#include "idk/core/stdmem.hpp"


namespace idk::engine
{
    struct Entity
    {
        uint32_t idx;
        uint32_t gen;
    };

    class EntityService: public idk::core::Service
    {
    private:
        const size_t mMaxEntities;
        uint32_t mNextIdx;
        uint32_t mFreeListSize;
        uint32_t *mFreeList;
        uint32_t *mGen;
        bool     *mAlive;

    public:
        EntityService();
        ~EntityService();
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;

        Entity createEntity();
        void destroyEntity(const Entity &E);
        bool isAlive(const Entity &E);
    };

}


