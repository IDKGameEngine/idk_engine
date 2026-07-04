#pragma once

#include <cstdint>

namespace idk::config::udp
{
    enum class PortNumber: uint16_t
    {
        Base = 4000,
        ClientHealth,
        ClientInput,
        Snapshot,
        ServerTimeSync,
        ClientTimeSync
    };
}
