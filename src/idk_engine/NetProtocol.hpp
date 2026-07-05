#pragma once

#include <cstddef>
#include <cstdint>
#include "idk_core/stdmem.hpp"

namespace idk::engine::NetProtocol
{
    enum class UdpTag: uint8_t
    {
        Invalid,
        RoundTripTime,
        UserCtrl,
        WorldEvent,
        EntityEvent
    };


    union Udpdata
    {
        UdpTag tag;
        uint8_t buf[512];

        template <typename T>
        uint8_t *write(uint8_t *dst, const T &src)
        {
            idk_memcpy(dst, &src, sizeof(T));
            return dst + sizeof(T);
        }

        template <typename T>
        uint8_t *read(uint8_t *src, T &dst)
        {
            idk_memcpy(&dst, src, sizeof(T));
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
            src = data.read(src, clientSendTime);
            src = data.read(src, serverSendTime);
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
            src = data.read(src, clientSendTime);
            src = data.read(src, seqNo);
            src = data.read(src, buttons);
            src = data.read(src, moveX);
            src = data.read(src, moveY);
            src = data.read(src, lookYaw);
            src = data.read(src, lookPitch);
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
            src = data.read(src, clientSendTime);
            src = data.read(src, seqNo);
            src = data.read(src, entityCount);
            for (int i=0; i<entityCount; i++)
            {
                src = data.read(src, entities[i]);
            }
        }
    };

    struct EntityEventData
    {
        static constexpr UdpTag UDP_TAG = UdpTag::EntityEvent;
        uint64_t clientSendTime;
        uint32_t entityId;
        uint32_t entityGen;

        void encode(Udpdata &data)
        {
            uint8_t *dst = data.buf;
            dst = data.write(dst, UDP_TAG);
            dst = data.write(dst, clientSendTime);
            dst = data.write(dst, entityId);
            dst = data.write(dst, entityGen);
        }

        void decode(Udpdata &data)
        {
            uint8_t *src = data.buf + sizeof(UdpTag);
            src = data.read(src, clientSendTime);
            src = data.read(src, entityId);
            src = data.read(src, entityGen);
        }
    };

}
