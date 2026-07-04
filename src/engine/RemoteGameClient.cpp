
#include "idk/engine/RemoteGameClient.hpp"
#include "idk/core/stdstr.hpp"


idk::engine::RemoteGameClient::RemoteGameClient()
:   mMostRecentPacketTime(0)
{
    // NET_GetAddressString
}


idk::engine::RemoteGameClient::~RemoteGameClient()
{

}


void idk::engine::RemoteGameClient::update()
{

}


bool idk::engine::RemoteGameClient::isTimedOut() const
{
    return mMostRecentPacketTime == 0;
}

