#include "idk_engine/EntityService.hpp"
#include "idk/core/New.hpp"


idk::engine::EntityService::EntityService()
:   IDK_SERVICE_CTOR(EntityService),
    mMaxEntities(mCfg["MAX_ENTITIES"].toU64()),
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


idk::engine::EntityService::~EntityService()
{

}


void idk::engine::EntityService::update(idk::IEngine*)
{

}


void idk::engine::EntityService::shutdown(idk::IEngine*)
{

}


idk::engine::Entity idk::engine::EntityService::createEntity()
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


void idk::engine::EntityService::destroyEntity(const Entity &E)
{
    if (isAlive(E))
    {
        const uint32_t idx = E.idx;
        mGen[idx] += 1;
        mAlive[idx] = false;
        mFreeList[mFreeListSize++] = idx;
    }
}


bool idk::engine::EntityService::isAlive(const Entity &E)
{
    const uint32_t idx = E.idx;
    return (idx < mMaxEntities) && (mAlive[idx]) && (mGen[idx] == E.gen);
}
