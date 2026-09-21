#pragma once

#include "idk/platform/AudioManager.hpp"
#include "idk/platform/EventManager.hpp"
#include "idk/platform/InputManager.hpp"
#include "idk/platform/VideoManager.hpp"
#include "libidk/dsa/Queue.hpp"


namespace idk
{
    class Engine;
    class EngineEvent;
    class EngineAPI;


    class EngineEvent
    {
    public:
        static constexpr int32_t T_Invalid   = 0;
        static constexpr int32_t T_EngineCtl = 1;
        static constexpr int32_t T_Platform  = 2;

        static constexpr int32_t S_Pause    = 1;
        static constexpr int32_t S_Resume   = 2;
        static constexpr int32_t S_Shutdown = 3;

        int32_t  type    = 0;
        int32_t  subtype = 0;
        uint64_t data    = 0UL;
    };


    class EngineAPI
    {
    public:
        ServiceManager *mOwner;
        AudioManager   *mAudio;
        EventManager   *mEvent;
        InputManager   *mInput;
        VideoManager   *mVideo;

        EngineAPI(ServiceManager*);
        EngineAPI(ServiceManager*, AudioManager*, EventManager*, InputManager*, VideoManager*);
        EngineAPI(const EngineAPI&) = default;
        EngineAPI(EngineAPI&&) = default;

        bool pushEvent(int32_t type, int32_t subtype=0UL, uint64_t data=0UL);
        void broadcastEvent(int32_t type, int32_t subtype=0UL, uint64_t data=0UL);

        template <typename ServiceType>
        ServiceType *getService() { return mOwner->getService<ServiceType>(); }

    private:
        friend class idk::Engine;
        idk::core::Queue<EngineEvent, 128> mEventQueue;

    };

}

