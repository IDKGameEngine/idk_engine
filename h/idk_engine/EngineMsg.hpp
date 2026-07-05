#pragma once

#include "idk/message/MessageRxTx.hpp"
#include "idk/message/EventChannel.hpp"

namespace idk
{
    struct EngineMsg
    {
        idk::MessageTxer *entityEventTxer;
        idk::MessageTxer *otherEventTxer;

        static EngineMsg &get()
        {
            static EngineMsg instance;
            return instance;
        }
    };
}

