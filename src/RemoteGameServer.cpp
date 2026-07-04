#include "idk_engine/RemoteGameServer.hpp"
#include "idk/core/log.hpp"
#include "idk/core/stdstr.hpp"


idk::engine::RemoteGameServer::RemoteGameServer(const char *hostname)
{
    mServerAddr = NET_ResolveHostname(hostname);
    if (!mServerAddr)
    {
        VLOG_FATAL("[UdpTxer::UdpTxer] Failure resolving host: %s", SDL_GetError());
    }
}


idk::engine::RemoteGameServer::~RemoteGameServer()
{
    NET_UnrefAddress((NET_Address*)mServerAddr);
}


void idk::engine::RemoteGameServer::update()
{

}
