#pragma once

#include "UdpAddress.hpp"

namespace idk::config::udp
{
    struct __attribute__((packed)) HeartbeatData
    {
        uint32_t seqNo;
        uint32_t clientSendTime;
    
        uint16_t buttons; // bitmask
        float moveX;
        float moveY;
        float lookYaw;
        float lookPitch;
    };

    using HeartbeatAddress = UdpAddress<HeartbeatData, udp::PortNumber::Heartbeat>;
}

