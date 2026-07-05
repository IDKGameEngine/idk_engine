#pragma once

#include "idk_core/MessageRxer.hpp"
#include "idk_core/MessageTxer.hpp"
#include "idk_core/assert.hpp"
#include "idk_core/stdmem.hpp"
#include "idk_core/types.hpp"
#include "idk_core/InplaceList.hpp"


namespace idk
{
    template <typename T>
    struct EventListener
    {
        void (*func)(void*, const T&);
        void *arg;
    };


    template <typename T>
    class EventChannel: private idk::NonMobile, public idk::MessageTxer
    {
    private:
        static constexpr size_t MAX_LISTENERS = 64;
        InplaceList<EventListener<T>, MAX_LISTENERS> mListeners;

    public:
        virtual bool sendMsg(const void *src, size_t size) override
        {
            alignas(T) static uint8_t msgData[sizeof(T)];
            IDK_ASSERT(size == sizeof(T), "size mismatch, possibly incorrect data type");
            idk_memcpy(msgData, src, size);

            for (EventListener<T> &obj: mListeners)
            {
                obj.func(obj.arg, *reinterpret_cast<T*>(msgData));
            }

            return (mListeners.size() > 0);
        }

        void subscribe(void (*func)(void*, const T&), void *arg)
        {
            mListeners.push({func, arg});
        }

    };


    // class EventBus
    // {

    // };

}

