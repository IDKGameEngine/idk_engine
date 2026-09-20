#pragma once

#include "libidk/dsa/Queue.hpp"


namespace idk
{
    struct EngineEvent
    {
        static constexpr int32_t PAUSE = 1;
        static constexpr int32_t RESUME = 2;
        static constexpr int32_t SHUTDOWN = 3;

        static idk::core::Queue<EngineEvent, 128> gEngineEventQueue;

        int32_t type = 0;
        int32_t data = 0;
    };


}

