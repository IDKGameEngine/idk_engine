#pragma once

#include "idk/core/types.hpp"

namespace idk::engine
{
    class UdpRxer
    {
    private:
        void *mSocket;
        uint16_t mPort;

    public:
        UdpRxer(uint16_t port);
        ~UdpRxer();
        void update();
        size_t recvmsg(void *buf, size_t bufsz);
    };

}

