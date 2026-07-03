#pragma once

#include "idk/core/types.hpp"

namespace idk
{
    class UdpTxer : private idk::NonCopyable, private idk::NonMovable
    {
    private:
        void *mSocket;
        void *mServerAddr;
        uint16_t mServerPort;

    public:
        UdpTxer(uint16_t serverPort);
        ~UdpTxer();
        int sendmsg(const void *buf, int bufsz);
    };
}

