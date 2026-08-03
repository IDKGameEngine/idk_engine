#include "idk_engine/NetService.hpp"


idk::NetService::NetService(idk::platform::Platform &plat)
:   IDK_SERVICE_CTOR(NetService),
    mPlat(plat)
{

}


idk::NetService::~NetService()
{

}


void idk::NetService::update(idk::IEngine *E)
{
    (void)E;
}


void idk::NetService::shutdown(idk::IEngine *E)
{
    (void)E;
}

