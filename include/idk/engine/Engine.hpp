#pragma once

#include "libidk/Types.hpp"
#include "libidk/platform/IPlatformContext.hpp"
#include "libidk/dsa/Queue.hpp"
#include <atomic>


namespace idk
{
    struct EngineContext
    {
        idk::core::Queue<int32_t, 64> testQueue;
    };


    class EngineComponent: public idk::Immobile
    {
    private:

    public:
        EngineComponent(EngineContext&) {  };
        virtual ~EngineComponent() = default;
        virtual void update() = 0;
    };


    class Engine: public idk::Immobile
    {
    private:
        static constexpr size_t MAX_COMPONENTS = 16;
        std::atomic_bool       mRunning;
        idk::EngineContext     mContext;
        idk::IPlatformContext *mPlat;
        size_t                 mComponentIdx;
        idk::EngineComponent  *mComponents[MAX_COMPONENTS];

    public:
        Engine(idk::IPlatformContext *plat);

        void start();

        template <typename ComponentType, typename... Args>
        void addComponent(Args&&... args)
        {
            IDK_ASSERT(mComponentIdx<MAX_COMPONENTS, "[Engine::addComponent] Too many Components!");
            mComponents[mComponentIdx++] = idk::New<ComponentType>(mContext, args...);
        }

        template <typename ComponentType>
        ComponentType *getComponent() noexcept
        {
            for (size_t i=0; i<mComponentIdx; i++)
            {
                if (ComponentType *srv = dynamic_cast<ComponentType*>(mComponents[i]))
                {
                    return srv;
                }
            }
            return nullptr;
        }
    };

}
