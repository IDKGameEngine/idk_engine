#pragma once

#include "idk_core/types.hpp"

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
        uint16_t mRemotePort;

    public:
        UdpRxTxer(uint16_t localPort=0);
        UdpRxTxer(const char *hostname, uint16_t hostport);
        ~UdpRxTxer();
    
        NET_Datagram *beginRecvMsg(void *buf, size_t bufsz);
        void endRecvMsg(NET_Datagram*);
        void replyMsg(NET_Datagram*, const void *data, size_t size);
        bool sendMsg(const void *data, size_t size);

    };

}

// #include "idk_core/message/MessageEndpoint.hpp"
// #include "idk_core/stdmem.hpp"

// class RemoteEndpointRxer: public idk::core::MessageEndpoint
// {
// private:
//     NET_DatagramSocket *mSocket;
//     NET_Address *mRemoteAddr;
//     uint16_t mRemotePort;

// public:
//     RemoteEndpointRxer(const char *hostname, uint16_t hostport)
//     :   MessageEndpoint(),
//         mSocket(NULL),
//         mRemoteAddr(NULL),
//         mRemotePort(hostport)
//     {
//         if (!(mSocket = NET_CreateDatagramSocket(NULL, hostport, 0)))
//         {
//             // VLOG_FATAL("[UdpRxTxer::UdpRxTxer] Failure creating socket: {}", SDL_GetError());
//         }

//         if (!(mRemoteAddr = NET_ResolveHostname(hostname)))
//         {
//             // VLOG_FATAL("[UdpRxTxer::UdpRxTxer] Failure resolving host: {}", SDL_GetError());
//         }
//     }

//     virtual size_t recvFrom(void *dst, size_t size) override
//     {
//         size_t res = 0;
//         NET_Datagram *dgram = NULL;
//         if (NET_ReceiveDatagram(mSocket, &dgram) && dgram)
//         {
//             size_t nbytes = static_cast<size_t>(dgram->buflen);
//             if (nbytes <= size)
//             {
//                 idk_memcpy(dst, dgram->buf, nbytes);
//                 res = nbytes;
//             }
//             NET_DestroyDatagram(dgram);
//         }
//         return res;
//     }

// };
