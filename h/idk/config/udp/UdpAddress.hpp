#pragma once

#include "UdpPortNumber.hpp"

namespace idk::config::udp
{
    template <typename TYPE, udp::PortNumber PORT>
    struct UdpAddress
    {
        using DataType = TYPE;
        static constexpr uint16_t PortNumber = static_cast<uint16_t>(PORT);
    };
}

