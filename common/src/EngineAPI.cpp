#include "idk/EngineAPI.hpp"


idk::EngineAPI::EngineAPI(ServiceManager *owner)
:   mOwner(owner),
    mAudio(nullptr),
    mEvent(nullptr),
    mInput(nullptr),
    mVideo(nullptr)
{

}


idk::EngineAPI::EngineAPI(ServiceManager *o, AudioManager *a, EventManager *e, InputManager *i, VideoManager *v)
:   mOwner(o),
    mAudio(a),
    mEvent(e),
    mInput(i),
    mVideo(v)
{

}


bool idk::EngineAPI::pushEvent(int32_t type, int32_t subtype, uint64_t data)
{
    return mEventQueue.push(EngineEvent{type, subtype, data});
}


void idk::EngineAPI::broadcastEvent(int32_t, int32_t, uint64_t data)
{
    mOwner->broadcastEvent(*this, (const void*)data);
}

