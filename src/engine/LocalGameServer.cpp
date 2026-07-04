#include <SDL3_net/SDL_net.h>

#include "idk/engine/LocalGameServer.hpp"


idk::engine::LocalGameServer::LocalGameServer()
{
    if (!NET_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}


idk::engine::LocalGameServer::~LocalGameServer()
{
    NET_Quit();
}


void idk::engine::LocalGameServer::update()
{
    // int size = static_cast<int>(mClients.size());
    // for (int i=size-1; i>=0; i--)
    // {
    //     RemoteGameClient &client = mClients[i];
    //     if (client.isTimedOut())
    //     {
    //         std::swap(mClients[i], mClients.back());
    //         mClients.pop();
    //     }
    // }

    for (RemoteGameClient &client: mClients)
    {
        client.update();
    }
}


// class UdpRxerHandshake: public idk::UdpRxerBase<UdpRxerHandshake>
// {
// public:
//     UdpRxerHandshake(): idk::UdpRxerBase(AddressType::PortNumber) {  };

//     void recvImpl(const NET_Datagram &dgram)
//     {
//         // NET_SendDatagram(mSocket, dgram.addr, dgram.port, buf, bufsz);
//     }
// };

