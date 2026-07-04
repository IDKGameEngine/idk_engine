#include <SDL3_net/SDL_net.h>

#include "idk_engine/UdpRxTxer.hpp"

#include "idk/core/stdmem.hpp"
#include "idk/core/stdstr.hpp"
#include "idk/core/math.hpp"
#include "idk/core/log.hpp"


idk::UdpRxer::UdpRxer(uint16_t localPort)
:   mLocalPort(localPort)
{
    mSocket = NET_CreateDatagramSocket(NULL, localPort, 0);
    if (!mSocket)
    {
        VLOG_FATAL("[UdpRxer::UdpRxer] Failure creating socket: %s", SDL_GetError());
    }

}


idk::UdpRxer::~UdpRxer()
{
    VLOG_INFO("[UdpRxer::~UdpRxer]");
    NET_DestroyDatagramSocket(mSocket);
}


NET_Datagram *idk::UdpRxer::beginRecvMsg(void *buf, size_t bufsz)
{
    NET_Datagram *dgram = NULL;
    if (NET_ReceiveDatagram(mSocket, &dgram) && dgram)
    {
        if (dgram->buflen > static_cast<int>(bufsz))
        {
            VLOG_WARN("[UdpRxer::beginRecvMsg] datagram->buflen > bufsz");
            return nullptr;
        }
        int nbytes = dgram->buflen;
        idk_memcpy(buf, dgram->buf, nbytes);
        return dgram;
    }
    return nullptr;
}


void idk::UdpRxer::endRecvMsg(NET_Datagram *dgram)
{
    NET_DestroyDatagram(dgram);
}


void idk::UdpRxer::replyMsg(NET_Datagram *dgram, const void *data, size_t size)
{
    if (!NET_SendDatagram(mSocket, dgram->addr, dgram->port, data, size))
    {
        VLOG_WARN("[UdpTxer::sendmsg] Failed to send datagram: %s", SDL_GetError());
    }
}




idk::UdpTxer::UdpTxer(const char *hostname, uint16_t hostport)
:    mRemotePort(hostport)
{
    mSocket = NET_CreateDatagramSocket(NULL, 0, 0);
    if (!mSocket)
    {
        VLOG_FATAL("[UdpTxer::UdpTxer] Failure creating socket: %s", SDL_GetError());
    }

    mRemoteAddr = NET_ResolveHostname(hostname);
    if (!mRemoteAddr)
    {
        VLOG_FATAL("[UdpTxer::UdpTxer] Failure resolving host: %s", SDL_GetError());
    }
}

idk::UdpTxer::~UdpTxer()
{
    NET_DestroyDatagramSocket((NET_DatagramSocket*)mSocket);
}

bool idk::UdpTxer::sendmsg(const void *data, size_t size)
{
    auto *sock = (NET_DatagramSocket*)mSocket;
    auto *addr = (NET_Address*)mRemoteAddr;
    if (!NET_SendDatagram(sock, addr, mRemotePort, data, size))
    {
        VLOG_WARN("[UdpTxer::sendmsg] Failed to send datagram: %s", SDL_GetError());
        return false;
    }
    return true;
}
