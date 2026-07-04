#pragma once

#include <cstddef>
#include <cstdint>

namespace idk::engine::NetProtocol
{
    static constexpr size_t MAX_CLIENTS = 32;


    enum class UdpTag: uint8_t
    {
        Invalid,
        RoundTripTime,
        UserCtrl,
        OtherType
    };

    struct RoundTripTimeData
    {
        static constexpr UdpTag TAG = UdpTag::RoundTripTime;
        uint64_t clientSendTime;
        uint64_t serverSendTime;
    };

    struct UserCtrlData
    {
        static constexpr UdpTag TAG = UdpTag::UserCtrl;
        uint64_t clientSendTime;
        uint32_t seqNo;
        uint16_t buttons; // bitmask
        float moveX;
        float moveY;
        float lookYaw;
        float lookPitch;
    };

    struct UdpPacket
    {
        UdpTag tag;
        union {
            RoundTripTimeData as_RoundTripTime;
            UserCtrlData as_UserCtrl;
        };
    };

}
