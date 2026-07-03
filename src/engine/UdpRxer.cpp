#include "idk/engine/UdpRxer.hpp"
#include "idk/core/stdmem.hpp"
#include "idk/core/math.hpp"
#include "idk/core/log.hpp"

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>

namespace idk
{
    UdpRxer::UdpRxer(uint16_t port)
    :   mPort(port)
    {
        mSocket = NET_CreateDatagramSocket(NULL, mPort, 0);
        if (!mSocket)
        {
            VLOG_FATAL("[UdpTxer::UdpTxer] Failure creating socket: %s", SDL_GetError());
        }

    }

    UdpRxer::~UdpRxer()
    {
        VLOG_INFO("[UdpRxer::~UdpRxer]");
        NET_DestroyDatagramSocket((NET_DatagramSocket*)mSocket);
    }

    int UdpRxer::recvmsg(void *buf, int bufsz)
    {
        NET_Datagram *dgram = NULL;

        while (NET_ReceiveDatagram((NET_DatagramSocket*)mSocket, &dgram))
        {
            if (!dgram)
            {
                return 0;
            }
            if (dgram->buflen > bufsz)
            {
                VLOG_WARN("[UdpRxer::recvmsg] datagram->buflen > bufsz");
                return 0;
            }

            // dgram->addr; dgram->port;
            int nbytes = dgram->buflen;
            idk_memcpy(buf, dgram->buf, nbytes);
            NET_DestroyDatagram(dgram);

            return nbytes;
        }

        return 0;
    }

    // void UdpRxer::update()
    // {
    //     NET_Datagram *dgram = NULL;
    //     while (NET_ReceiveDatagram((NET_DatagramSocket*)mSocket, &dgram))
    //     {
    //         if (!dgram) { break; }
    //         // VLOG_INFO("[UdpRxer] recveived {} bytes", dgram->buflen);
    //         // packetHandler(dgram);
    //         // Can inspect dgram->addr and dgram->port for sender information
    //         NET_DestroyDatagram(dgram);
    //     }
    // }
}