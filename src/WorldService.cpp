#include "idk_engine/WorldService.hpp"
#include "idk_config/Memory.hpp"
#include "idk/core/stdmem.hpp"


idk::engine::WorldService::WorldService()
:   IDK_SERVICE_CTOR(WorldService),
    mEntityManager(mCfg["MAX_ENTITIES"].toU64())
{

}


idk::engine::WorldService::~WorldService()
{

}


void idk::engine::WorldService::update(idk::IEngine*)
{

}


void idk::engine::WorldService::shutdown(idk::IEngine*)
{

}

