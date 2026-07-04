#pragma once

#include "idk/core/types.hpp"

namespace idk
{
    class UdpTxer: private idk::NonCopyable, private idk::NonMovable
    {
    private:
        void *mSocket;
        void *mServerAddr;
        uint16_t mServerPort;

    public:
        UdpTxer(uint16_t serverPort);
        ~UdpTxer();
        int sendmsg(const void *buf, int bufsz);
    };


    template <typename AddressType>
    class UdpTxer2: protected idk::UdpTxer
    {
    public:
        using DataType = AddressType::DataType;
        UdpTxer2(): UdpTxer(AddressType::PortNumber) {  };

        bool sendmsg(const DataType &data)
        {
            return (0 != UdpTxer::sendmsg(&data, sizeof(DataType)));
        }
    };

}

