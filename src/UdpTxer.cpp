#include "idk/engine/UdpTxer.hpp"
#include "idk/core/log.hpp"

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>

namespace idk::engine
{
    UdpTxer::UdpTxer(uint16_t serverPort)
    :   mSocket(NET_CreateDatagramSocket(NULL, 0, 0)),
        mServerAddr(NET_ResolveHostname("127.0.0.1")),
        mServerPort(serverPort)
    {
        if (!mServerAddr)
        {
            VLOG_FATAL("[UdpTxer::UdpTxer] Failed to resolve host: %s", SDL_GetError());
        }
    }

    UdpTxer::~UdpTxer()
    {
        NET_DestroyDatagramSocket((NET_DatagramSocket*)mSocket);
    }

    size_t UdpTxer::sendmsg(const void *buf, size_t bufsz)
    {
        auto *sock = (NET_DatagramSocket*)mSocket;
        auto *addr = (NET_Address *)mServerAddr;

        if (!NET_SendDatagram(sock, addr, mServerPort, buf, bufsz))
        {
            SDL_Log("Failed to send datagram: %s", SDL_GetError());
        }
    }

}