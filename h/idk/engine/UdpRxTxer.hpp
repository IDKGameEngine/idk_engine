#pragma once

#include <SDL3_net/SDL_net.h>

#include "idk/core/log.hpp"
#include "idk/core/types.hpp"

namespace idk
{
    template <typename Impl>
    class UdpRxTxer: private idk::NonCopyable, private idk::NonMovable
    {
    protected:
        NET_DatagramSocket *mSocket;
        NET_Address *mServerAddr;
        uint16_t mServerPort;

    public:
        UdpRxTxer(uint16_t serverPort)
        :    mServerPort(serverPort)
        {
            mSocket = NET_CreateDatagramSocket(NULL, 0, 0);
            if (!mSocket)
            {
                VLOG_FATAL("[UdpRxTxer::UdpRxTxer] Failure creating socket: %s", SDL_GetError());
            }

            mServerAddr = NET_ResolveHostname("127.0.0.1");
            if (!mServerAddr)
            {
                VLOG_FATAL("[UdpRxTxer::UdpRxTxer] Failure resolving host: %s", SDL_GetError());
            }
        }

        ~UdpRxTxer()
        {
            NET_DestroyDatagramSocket(mSocket);
        }

        int sendmsg(const void *buf, int bufsz)
        {
            if (!NET_SendDatagram(mSocket, mServerAddr, mServerPort, buf, bufsz))
            {
                VLOG_WARN("[UdpRxTxer::sendmsg] Failed to send datagram: %s", SDL_GetError());
                return 0;
            }

            return bufsz;
        }

        void update()
        {
            NET_Datagram *dgram = NULL;

            while (NET_ReceiveDatagram(mSocket, &dgram))
            {
                if (!dgram)
                {
                    return 0;
                }
                if (dgram->buflen > bufsz)
                {
                    VLOG_WARN("[UdpRxTxer::recvmsg] datagram->buflen > bufsz");
                    return 0;
                }

                static_cast<Impl*>(this)->onRecvImpl(dgram);
                NET_DestroyDatagram(dgram);

                return nbytes;
            }  
        }
    };

}
