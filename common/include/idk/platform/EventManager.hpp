#pragma once

#include "libidk/Service.hpp"


namespace idk
{
    class EventManager: public idk::Service
    {
    private:
        using EventCallback = void (*)(const void *event);
        idk::InplaceList<uintptr_t, 64> mEventCallbacks;

    public:
        virtual void onInit(EngineAPI&) final;
        virtual void onShutdown(EngineAPI&) final;
        virtual void onUpdate(EngineAPI&) final;

        bool addEventCallback(EventCallback func)
        {
            if (!mEventCallbacks.full())
            {
                mEventCallbacks.push(reinterpret_cast<uintptr_t>(func));
                return true;
            }
            return false;
        }

    };
}

