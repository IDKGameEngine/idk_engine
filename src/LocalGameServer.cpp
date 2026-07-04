#include "idk_engine/LocalGameServer.hpp"
#include "idk/core/log.hpp"

#include <thread>


void idk::engine::LocalGameServer::udpListenFunc(LocalGameServer *server)
{
    while (server->isActive())
    {
        // server->mClients[0].
    }
}


idk::engine::LocalGameServer::LocalGameServer()
:   mActive{true}
{
    if (!NET_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }

    std::thread(LocalGameServer::udpListenFunc, this).detach();
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

