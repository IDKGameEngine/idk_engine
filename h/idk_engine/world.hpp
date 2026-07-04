#pragma once

#include "idk/core/InplaceList.hpp"
#include "idk/core/stdmem.hpp"

namespace idk
{
    struct GameObject
    {
        const uint32_t idx;
        const uint32_t gen;
        GameObject(uint32_t i=0, uint32_t g=0) : idx(i), gen(g) {  };
    };

    static constexpr size_t MAX_OBJECTS = 512;

    class GameObjectManager
    {
    private:
        uint32_t nextIdx_;
        uint32_t freeListSize_;
        uint32_t freeList_[MAX_OBJECTS];
        uint32_t gen_[MAX_OBJECTS];
        bool alive_[MAX_OBJECTS];

    public:
        GameObjectManager()
        :   nextIdx_(0), freeListSize_(0)
        {
            idk_memset(freeList_, 0, sizeof(freeList_));
            idk_memset(gen_, 0, sizeof(gen_));
            idk_memset(alive_, 0, sizeof(alive_));
        }

        bool isAlive(const GameObject &obj)
        {
            const uint32_t idx = obj.idx;
            return (idx< MAX_OBJECTS) && (alive_[idx]) && (gen_[idx] == obj.gen);
        }

        GameObject createObject()
        {
            uint32_t idx;
            if (freeListSize_ > 0)
            {
                idx = freeList_[--freeListSize_];
            }
            else
            {
                idx = nextIdx_++;
            }
            alive_[idx] = true;

            return GameObject(idx, gen_[idx]);
        }

        void destroyObject(const GameObject &obj)
        {
            if (isAlive(obj))
            {
                gen_[obj.idx] += 1;
                alive_[obj.idx] = false;
                freeList_[freeListSize_++] = obj.idx;
            }
        }
    };


    class GameScene
    {
    public:
        static constexpr size_t MAX_OBJECTS = 1024;
        GameScene();
        
    private:
        idk::InplaceList<GameObject, MAX_OBJECTS> mObjects;

    };


    class GameWorld
    {
    public:
        GameWorld();
        
    private:

    };
}


