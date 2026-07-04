#pragma once

#include "idk/core/log.hpp"
#include "idk/core/types.hpp"

#include "idk/config/udp/ClientHealthAddress.hpp"
#include "idk/engine/UdpRxTxer.hpp"


namespace idk::engine
{
    class ClientHealthRxTxer: public idk::UdpRxTxer<ClientHealthRxTxer>
    {
    public:
        ClientHealthRxTxer()
        :   UdpRxTxer(config::udp::ClientHealthAddress::PortNumber)
        {
            
        }

        void recvImpl(const NET_Datagram &dgram)
        {
            // NET_SendDatagram(mSocket, dgram.addr, dgram.port, buf, bufsz);
        }
    };


    class RemoteGameClient: private idk::NonMobile
    {
    public:
        static constexpr size_t MAX_HOSTNAME_LENGTH = 256;
        RemoteGameClient();
        ~RemoteGameClient();
        void update();
        bool isTimedOut() const;

    private:
        uint64_t mMostRecentPacketTime;
        char mHostname[MAX_HOSTNAME_LENGTH];

    };

}

