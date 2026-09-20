#pragma once

#include "libidk/Types.hpp"
#include "libidk/platform/IPlatformContext.hpp"
#include "libidk/dsa/Queue.hpp"
#include "libidk/dsa/List.hpp"
#include <atomic>


namespace idk
{
    struct EngineContext;
    class EngineService;
    class Engine;


    struct EngineContext
    {
        idk::core::Queue<int32_t, 64> testQueue;
    };


    class EngineService: public idk::Immobile
    {
    private:

    public:
        EngineService(EngineContext&) {  };
        virtual ~EngineService() = default;
        virtual void init(idk::Engine&) = 0;
        virtual void update(idk::Engine&) = 0;
    };


    class Engine: public idk::Immobile
    {
    private:
        static constexpr size_t MAX_COMPONENTS = 16;
        std::atomic_bool       mRunning;
        idk::EngineContext     mContext;
        idk::IPlatformContext *mPlat;
        idk::InplaceList<EngineService*, MAX_COMPONENTS> mComponents;

    public:
        Engine(idk::IPlatformContext *plat);

        void start();

        idk::IPlatformContext *getPlatformContext() { return mPlat; }

        template <typename ComponentType, typename... Args>
        void addComponent(Args&&... args)
        {
            // IDK_ASSERT(mComponentIdx<MAX_COMPONENTS, "[Engine::addComponent] Too many Components!");
            // mComponents[mComponentIdx++] = idk::New<ComponentType>(mContext, args...);
            mComponents.push(idk::New<ComponentType>(mContext, args...));
        }

        template <typename ComponentType>
        ComponentType *getComponent() noexcept
        {
            for (EngineService *C: mComponents)
            {
                if (ComponentType *ptr = dynamic_cast<ComponentType*>(C))
                {
                    return ptr;
                }
            }
            return nullptr;
        }
    };

}
