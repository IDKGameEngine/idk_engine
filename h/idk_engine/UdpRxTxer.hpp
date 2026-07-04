#pragma once

#include "idk/core/types.hpp"

struct NET_DatagramSocket;
struct NET_Datagram;
struct NET_Address;


namespace idk
{
    class UdpRxer: private idk::NonMobile
    {
    private:
        NET_DatagramSocket *mSocket;
        uint16_t mLocalPort;

    public:
        UdpRxer(uint16_t localPort = 0);
        ~UdpRxer();
        NET_Datagram *beginRecvMsg(void *buf, size_t bufsz);
        void endRecvMsg(NET_Datagram*);
        void replyMsg(NET_Datagram*, const void *data, size_t size);

    };

    class UdpTxer: private idk::NonMobile
    {
    private:
        static constexpr size_t MAX_HOSTNAME_LENGTH = 128;
        void *mSocket;
        void *mRemoteAddr;
        uint16_t mRemotePort;
        char mHostname[MAX_HOSTNAME_LENGTH];

    public:
        UdpTxer(const char *hostname, uint16_t hostport);
        ~UdpTxer();
        bool sendmsg(const void *data, size_t size);
    };
}
