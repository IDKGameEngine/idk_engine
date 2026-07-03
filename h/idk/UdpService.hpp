#pragma once

#include "idk/core/engine.hpp"
#include "idk/core/file.hpp"
#include "idk/core/InplaceList.hpp"

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>

namespace idk
{
    class UdpRxer
    {
    private:
        NET_DatagramSocket *mSocket;
        uint16_t mPort;

    public:
        UdpRxer(uint16_t port)
        :   mSocket(NET_CreateDatagramSocket(NULL, port, 0)),
            mPort(port)
        {

        }

        ~UdpRxer() { NET_DestroyDatagramSocket(mSocket); }

        void update()
        {
            NET_Datagram *dgram = NULL;
            while (NET_ReceiveDatagram(mSocket, &dgram))
            {
                if (!dgram) { break; }
                VLOG_INFO("[UdpRxer] recveived {} bytes", dgram->buflen);
                // packetHandler(dgram);
                // Can inspect dgram->addr and dgram->port for sender information
                NET_DestroyDatagram(dgram);
            }
        }
    };


    class UdpService: public idk::core::Service
    {
    public:
        UdpService();
        virtual void startup(idk::IEngine*) final;
        virtual void update(idk::IEngine*) final;
        virtual void shutdown(idk::IEngine*) final;

    private:
        idk::RaiiFunc<void()> mRaii;
        idk::InplaceList<UdpRxer, 4> mRxers;
        // UdpReceiver mIoCtrl;
        // UdpReceiver mUdpCtrl;
        // UdpReceiver mEngineCtrl;
        // NET_DatagramSocket *mSocketIoCtrl;
        // NET_DatagramSocket *mSocketUdpCtrl;
        // NET_DatagramSocket *mSocketEngineCtrl;
    };

}

