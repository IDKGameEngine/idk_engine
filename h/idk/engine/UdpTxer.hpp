#pragma once

#include "idk/core/types.hpp"

namespace idk::engine
{
    class UdpTxer
    {
    private:
        void *mSocket;
        void *mServerAddr;
        uint16_t mServerPort;

    public:
        UdpTxer(uint16_t serverPort);
        ~UdpTxer();
        size_t sendmsg(const void *buf, size_t bufsz);
    };
}

