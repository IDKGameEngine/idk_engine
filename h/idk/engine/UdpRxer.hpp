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
