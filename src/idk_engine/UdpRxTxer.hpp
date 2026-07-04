#pragma once

#include "idk/core/types.hpp"

struct NET_DatagramSocket;
struct NET_Datagram;
struct NET_Address;


namespace idk
{
    class UdpRxTxer: private idk::NonMobile
    {
    private:
        NET_DatagramSocket *mSocket;
        NET_Address *mRemoteAddr;
        uint16_t mLocalPort;
        uint16_t mRemotePort;

    public:
        UdpRxTxer(uint16_t localPort=0);
        ~UdpRxTxer();
    
        bool setRemote(const char *hostname, uint16_t port);

        NET_Datagram *beginRecvMsg(void *buf, size_t bufsz);
        void endRecvMsg(NET_Datagram*);
        void replyMsg(NET_Datagram*, const void *data, size_t size);
        bool sendMsg(const void *data, size_t size);

    };

}
