#include "idk/engine/UdpRxer.hpp"

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>

namespace idk::engine
{
    UdpRxer::UdpRxer(uint16_t port)
    :   mSocket(NET_CreateDatagramSocket(NULL, port, 0)),
        mPort(port)
    {

    }

    UdpRxer::~UdpRxer()
    {
        NET_DestroyDatagramSocket((NET_DatagramSocket*)mSocket);
    }

    size_t UdpRxer::recvmsg(void *buf, size_t bufsz)
    {
        
    }


    void UdpRxer::update()
    {
        NET_Datagram *dgram = NULL;
        while (NET_ReceiveDatagram((NET_DatagramSocket*)mSocket, &dgram))
        {
            if (!dgram) { break; }
            // VLOG_INFO("[UdpRxer] recveived {} bytes", dgram->buflen);
            // packetHandler(dgram);
            // Can inspect dgram->addr and dgram->port for sender information
            NET_DestroyDatagram(dgram);
        }
    }
}