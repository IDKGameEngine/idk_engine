#pragma once

#include <cstddef>
#include <cstdint>

namespace idk::config::network
{
    static constexpr size_t MAX_CLIENTS = 32;
    static constexpr uint16_t SERVER_PORT = 7777;

    struct UdpRttPacket
    {
        uint64_t clientSendTime;
        uint64_t serverSendTime;
    };

    struct __attribute__((packed)) UdpInputPacket
    {
        uint32_t seqNo;
        uint32_t clientSendTime;
        uint16_t buttons; // bitmask
        float moveX;
        float moveY;
        float lookYaw;
        float lookPitch;
    };
}
