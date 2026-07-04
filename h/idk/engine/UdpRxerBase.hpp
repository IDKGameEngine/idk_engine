#pragma once

#include "idk/core/log.hpp"
#include "idk/core/types.hpp"

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>


namespace idk
{
    template <typename UdpRxerImpl>
    class UdpRxerBase: private idk::NonCopyable, private idk::NonMovable
    {
    private:
        NET_DatagramSocket *mSocket;
        uint16_t mPort;

    public:
        UdpRxerBase(uint16_t port)
        :   mPort(port)
        {
            mSocket = NET_CreateDatagramSocket(NULL, mPort, 0);
            if (!mSocket)
            {
                VLOG_FATAL("[UdpTxer::UdpTxer] Failure creating socket: %s", SDL_GetError());
            }
        }

        ~UdpRxerBase()
        {
            NET_DestroyDatagramSocket(mSocket);
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
                    VLOG_WARN("[UdpRxer::recvmsg] datagram->buflen > bufsz");
                    return 0;
                }

                static_cast<UdpRxerImpl*>(this)->onRecvImpl(dgram);
                NET_DestroyDatagram(dgram);

                return nbytes;
            }  
        }
    };


}

