#pragma once

#include "UdpAddress.hpp"

namespace idk::config::udp
{
    struct __attribute__((packed)) ClientHealthData
    {
        uint64_t clientSendTime;
        uint64_t serverRecvTime;
        uint64_t serverSendTime;
    };

    using ClientHealthAddress = UdpAddress<ClientHealthData, udp::PortNumber::ClientHealth>;
}

