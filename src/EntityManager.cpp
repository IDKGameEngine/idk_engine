#include "idk_engine/EntityManager.hpp"
#include "idk_config/Memory.hpp"
#include "idk/core/stdmem.hpp"


idk::engine::EntityManager::EntityManager(size_t maxEntities)
:   mMaxEntities(maxEntities),
    mNextIdx(0),
    mFreeListSize(0),
    mFreeList(idk::NewArray<uint32_t>(mMaxEntities)),
    mGen(idk::NewArray<uint32_t>(mMaxEntities)),
    mAlive(idk::NewArray<bool>(mMaxEntities))
{
    idk_memset(mFreeList, 0, mMaxEntities);
    idk_memset(mGen, 0, mMaxEntities);
    idk_memset(mAlive, 0, mMaxEntities);
}



idk::engine::Entity idk::engine::EntityManager::createEntity()
{
    uint32_t idx;
    if (mFreeListSize > 0)
    {
        idx = mFreeList[--mFreeListSize];
    }
    else
    {
        idx = mNextIdx++;
    }
    mAlive[idx] = true;
    return Entity{idx, mGen[idx]};
}


void idk::engine::EntityManager::destroyEntity(const Entity &E)
{
    if (isAlive(E))
    {
        const uint32_t idx = E.idx;
        mGen[idx] += 1;
        mAlive[idx] = false;
        mFreeList[mFreeListSize++] = idx;
    }
}


bool idk::engine::EntityManager::isAlive(const Entity &E)
{
    const uint32_t idx = E.idx;
    return (idx < mMaxEntities) && (mAlive[idx]) && (mGen[idx] == E.gen);
}
