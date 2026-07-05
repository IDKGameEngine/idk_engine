#include "idk_engine/NetService.hpp"
#include "idk_engine/GameClient.hpp"
#include "idk_engine/GameServer.hpp"
#include "idk_config/Memory.hpp"
#include "idk_core/stdmem.hpp"

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>

#include <thread>


idk::engine::NetService::NetService()
:   IDK_SERVICE_CTOR(NetService),
    mServerPort(mCfg["SERVER_PORT"].toU16()),
    mGameClient(nullptr),
    mGameServer(nullptr)
{
    if (!SDL_Init(SDL_INIT_EVENTS))
    {
        VLOG_FATAL("[NetService] {}", SDL_GetError());
    }

    if (!NET_Init())
    {
        VLOG_FATAL("[NetService] {}", SDL_GetError());
    }
}

idk::engine::NetService::~NetService()
{
    NET_Quit();
}


void idk::engine::NetService::update(idk::IEngine *E)
{
    if (mGameClient) { mGameClient->update(E); }
    if (mGameServer) { mGameServer->update(E); }
}

void idk::engine::NetService::shutdown(idk::IEngine*)
{

}


bool idk::engine::NetService::startGameClient()
{
    if (mGameClient) { return false; }
    mGameClient = idk::New<GameClient>(mServerPort);
    return true;
}


bool idk::engine::NetService::startGameServer()
{
    if (mGameServer) { return false; }
    mGameServer = idk::New<GameServer>(mServerPort);
    return true;
}



