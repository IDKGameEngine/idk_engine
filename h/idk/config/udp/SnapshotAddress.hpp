#pragma once

#include "UdpAddress.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace idk::config::udp
{
    struct __attribute__((packed)) EntityState
    {
        uint32_t id;
        glm::vec3 pos;
        glm::quat rot;
        uint16_t flags;
    };

    struct __attribute__((packed)) SnapshotData
    {
        static constexpr size_t MAX_ENTITIES_PER_PACKET = 16;
        uint32_t serverTickNo;
        uint32_t serverSendTime;
        uint32_t lastKnownClientTime;
        uint16_t entityCount;
        EntityState entityStates[MAX_ENTITIES_PER_PACKET];
    };

    using SnapshotAddress = UdpAddress<SnapshotData, udp::PortNumber::Snapshot>;
}

