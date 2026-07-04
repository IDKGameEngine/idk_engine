#pragma once

#include "idk/core/types.hpp"

namespace idk
{
    class UdpRxer: private idk::NonCopyable, private idk::NonMovable
    {
    private:
        void *mSocket;
        uint16_t mPort;

    public:
        UdpRxer(uint16_t port);
        ~UdpRxer();
        void update();
        int recvmsg(void *buf, int bufsz);
    };

    template <typename AddressType>
    class UdpRxer2: protected idk::UdpRxer
    {
    public:
        using DataType = AddressType::DataType;
        UdpRxer2(): UdpRxer(AddressType::PortNumber) {  };

        bool recvmsg(DataType &data)
        {
            return (0 != UdpRxer::recvmsg(&data, sizeof(DataType)));
        }
    };

}

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>

namespace idk
{
    template <typename T>
    struct UdpRxData
    {
        T data;
    };

    template <typename Impl>
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
                if (!dgram) { break; }
                static_cast<UdpRxerImpl*>(this)->onRecvImpl(*dgram);
                NET_DestroyDatagram(dgram);
            }  
        }
    };

}


template <typename AddressType>
class UdpRxerHandshake: public idk::UdpRxerBase<UdpRxerHandshake>
{
public:
    UdpRxerHandshake(): idk::UdpRxerBase(AddressType::PortNumber) {  };
    
};
