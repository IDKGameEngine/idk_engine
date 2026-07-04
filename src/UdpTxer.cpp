#include "idk_engine/UdpTxer.hpp"
#include "idk/core/log.hpp"

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>

namespace idk
{
    UdpTxer::UdpTxer(uint16_t serverPort)
    :    mServerPort(serverPort)
    {
        mSocket = NET_CreateDatagramSocket(NULL, 0, 0);
        if (!mSocket)
        {
            VLOG_FATAL("[UdpTxer::UdpTxer] Failure creating socket: %s", SDL_GetError());
        }

        mServerAddr = NET_ResolveHostname("127.0.0.1");
        if (!mServerAddr)
        {
            VLOG_FATAL("[UdpTxer::UdpTxer] Failure resolving host: %s", SDL_GetError());
        }
    }

    UdpTxer::~UdpTxer()
    {
        NET_DestroyDatagramSocket((NET_DatagramSocket*)mSocket);
    }

    int UdpTxer::sendmsg(const void *buf, int bufsz)
    {
        auto *sock = (NET_DatagramSocket*)mSocket;
        auto *addr = (NET_Address*)mServerAddr;

        if (!NET_SendDatagram(sock, addr, mServerPort, buf, bufsz))
        {
            VLOG_WARN("[UdpTxer::sendmsg] Failed to send datagram: %s", SDL_GetError());
            return 0;
        }

        return bufsz;
    }

}