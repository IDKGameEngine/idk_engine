#pragma once

#include "UdpAddress.hpp"

namespace idk::config::udp
{
    struct __attribute__((packed)) TimeSyncData
    {
        uint64_t clientSendTime;
        uint64_t serverSendTime;
    };

    using ServerTimeSyncAddress = UdpAddress<TimeSyncData, udp::PortNumber::ServerTimeSync>;
    using ClientTimeSyncAddress = UdpAddress<TimeSyncData, udp::PortNumber::ClientTimeSync>;
}

