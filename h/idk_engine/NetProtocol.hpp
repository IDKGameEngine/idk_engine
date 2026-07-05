#pragma once

#include <cstddef>
#include <cstdint>
#include "idk/core/stdmem.hpp"

namespace idk::engine::NetProtocol
{
    enum class UdpTag: uint8_t
    {
        Invalid,
        RoundTripTime,
        UserCtrl,
        WorldEvent
    };


    union Udpdata
    {
        UdpTag tag;
        uint8_t buf[512];

        template <typename T>
        uint8_t *write(uint8_t *dst, const T &value)
        {
            idk_memcpy(dst, &value, sizeof(T));
            return dst + sizeof(T);
        }

        template <typename T>
        uint8_t *read(T &value, uint8_t *src)
        {
            idk_memcpy(&value, src, sizeof(T));
            return src + sizeof(T);
        }
    };

    struct RoundTripTimeData
    {
        static constexpr UdpTag UDP_TAG = UdpTag::RoundTripTime;
        uint64_t clientSendTime;
        uint64_t serverSendTime;

        void encode(Udpdata &data)
        {
            uint8_t *dst = data.buf;
            dst = data.write(dst, UDP_TAG);
            dst = data.write(dst, clientSendTime);
            dst = data.write(dst, serverSendTime);
        }

        void decode(Udpdata &data)
        {
            uint8_t *src = data.buf + sizeof(UdpTag);
            src = data.read(clientSendTime, src);
            src = data.read(serverSendTime, src);
        }
    };

    struct UserCtrlData
    {
        static constexpr UdpTag UDP_TAG = UdpTag::UserCtrl;
        uint64_t clientSendTime;
        uint32_t seqNo;
        uint16_t buttons; // bitmask
        float moveX;
        float moveY;
        float lookYaw;
        float lookPitch;

        void encode(Udpdata &data)
        {
            uint8_t *dst = data.buf;
            dst = data.write(dst, UDP_TAG);
            dst = data.write(dst, clientSendTime);
            dst = data.write(dst, seqNo);
            dst = data.write(dst, buttons);
            dst = data.write(dst, moveX);
            dst = data.write(dst, moveY);
            dst = data.write(dst, lookYaw);
            dst = data.write(dst, lookPitch);
        }

        void decode(Udpdata &data)
        {
            uint8_t *src = data.buf + sizeof(UdpTag);
            src = data.read(clientSendTime, src);
            src = data.read(seqNo, src);
            src = data.read(buttons, src);
            src = data.read(moveX, src);
            src = data.read(moveY, src);
            src = data.read(lookYaw, src);
            src = data.read(lookPitch, src);
        }
    };

    struct WorldEventData
    {
        static constexpr UdpTag UDP_TAG = UdpTag::WorldEvent;
        uint64_t clientSendTime;
        uint32_t seqNo;
        uint8_t  entityCount;
        uint64_t entities[16];

        void encode(Udpdata &data)
        {
            uint8_t *dst = data.buf;
            dst = data.write(dst, UDP_TAG);
            dst = data.write(dst, clientSendTime);
            dst = data.write(dst, seqNo);
            dst = data.write(dst, entityCount);
            for (int i=0; i<entityCount; i++)
            {
                dst = data.write(dst, entities[i]);
            }
        }

        void decode(Udpdata &data)
        {
            uint8_t *src = data.buf + sizeof(UdpTag);
            src = data.read(clientSendTime, src);
            src = data.read(seqNo, src);
            src = data.read(entityCount, src);
            for (int i=0; i<entityCount; i++)
            {
                src = data.read(entities[i], src);
            }
        }
    };

}
