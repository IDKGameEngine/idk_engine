#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class EventManager: public idk::IPlatformFeature
    {
    private:
        using EventCallback = void (*)(const void *event);
        idk::InplaceList<uintptr_t, 64> mEventCallbacks;

    public:
        EventManager(PlatformContext &ctx);

        virtual void onInit(ServiceManager*) final;
        virtual void onUpdate(ServiceManager*) final;

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

