#pragma once

#include "idk/core/types.hpp"

namespace idk
{
    class UdpRxer : private idk::NonCopyable, private idk::NonMovable
    {
    private:
        void *mSocket;
        uint16_t mPort;

    public:
        UdpRxer(uint16_t port);
        ~UdpRxer();
        void update();
        int recvmsg(void *buf, int bufsz);
    };

}

