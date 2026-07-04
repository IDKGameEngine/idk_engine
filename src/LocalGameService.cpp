#include "idk_engine/LocalGameService.hpp"
#include "idk/core/stdmem.hpp"

idk::engine::LocalGameClient::LocalGameClient()
:   mHealthTimer(5),
    mInputTimer(16)
{
    idk_memset(&mHealthData, 0, sizeof(mHealthData));
    idk_memset(&mInputData, 0, sizeof(mInputData));
}

void idk::engine::LocalGameClient::update()
{
    if (mHealthTimer.expired())
    {
        mHealthTimer.reset();
        mHealthTx.sendmsg(mHealthData);
    }

    if (mInputTimer.expired())
    {
        mInputTimer.reset();
        mInputTx.sendmsg(mInputData);
    }
}




static void LocalGameServiceRaiiFunc()
{
    if (!NET_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }
}

idk::engine::LocalGameService::LocalGameService()
:   IDK_SERVICE_CTOR(LocalGameService),
    mRaii(LocalGameServiceRaiiFunc),
    mLocalClient()
{
    
}

void idk::engine::LocalGameService::update(idk::IEngine*)
{

}

void idk::engine::LocalGameService::shutdown(idk::IEngine*)
{

}
