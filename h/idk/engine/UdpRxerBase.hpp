#pragma once

#include "idk/core/log.hpp"
#include "idk/core/types.hpp"

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>


namespace idk
{
    template <typename Impl>
    class UdpRxerBase: private idk::NonCopyable, private idk::NonMovable
    {
    protected:
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

                static_cast<Impl*>(this)->recvImpl(dgram);
                NET_DestroyDatagram(dgram);

                return nbytes;
            }  
        }
    };

}



template <typename AddressType>
class UdpRxerHandshake: public idk::UdpRxerBase<UdpRxerHandshake>
{
public:
    UdpRxerHandshake(): idk::UdpRxerBase(AddressType::PortNumber) {  };

    void recvImpl(const NET_Datagram &dgram)
    {
        // NET_SendDatagram(mSocket, dgram.addr, dgram.port, buf, bufsz);
    }
};

