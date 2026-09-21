#include "idk/EngineAPI.hpp"


idk::EngineAPI::EngineAPI(ServiceManager *o, AudioManager *a, EventManager *e, InputManager *i, VideoManager *v)
:   mOwner(o),
    mAudio(a),
    mEvent(e),
    mInput(i),
    mVideo(v)
{

}


void idk::EngineAPI::dispatchEvent(int32_t type, int32_t subtype, uint64_t data)
{
    if (type == EngineEvent::T_Platform)
    {
        mOwner->dispatchEvent(*this, reinterpret_cast<const void*>(data));
    }
    else
    {
        mEventQueue.push(EngineEvent{type, subtype, data});
    }
}


double idk::EngineAPI::getDeltaTimeSec()
{
    return mDeltaTimeSec;
}

double idk::EngineAPI::getFixedDeltaTimeSec()
{
    return mFixedDeltaTimeSec;
}

