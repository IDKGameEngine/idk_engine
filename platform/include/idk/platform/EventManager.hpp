#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class EventManager: public idk::IPlatformFeature
    {
    private:
        // static constexpr size_t MAX_CALLBACKS = 64;
        // using EventCallback = void (*)(IEventManager*, void *event);
        // idk::InplaceList<uintptr_t, MAX_CALLBACKS> mEventFuncs;

    public:
        EventManager(PlatformContext &ctx);

        virtual void onInit(ServiceManager*) final;
        virtual void onUpdate(ServiceManager*) final;

        // bool addEventCallback(EventCallback func)
        // {
        //     if (!mEventFuncs.full())
        //     {
        //         mEventFuncs.push(reinterpret_cast<uintptr_t>(func));
        //         return true;
        //     }
        //     return false;
        // }

    };
}

