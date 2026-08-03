#include "idk/engine/AudioService.hpp"


idk::AudioService::AudioService(idk::platform::Platform &plat)
:   IDK_SERVICE_CTOR(AudioService),
    mPlat(plat)
{

}


idk::AudioService::~AudioService()
{

}


void idk::AudioService::update(idk::IEngine *E)
{
    (void)E;
}


void idk::AudioService::shutdown(idk::IEngine *E)
{
    (void)E;
}

