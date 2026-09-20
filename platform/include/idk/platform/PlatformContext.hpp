#pragma once

#include "libidk/Types.hpp"
#include "libidk/Assert.hpp"
#include "libidk/dsa/List.hpp"
#include "libidk/New.hpp"
#include <atomic>


namespace idk
{
    class IPlatformFeature;


    struct PlatformConfig
    {
        const char* windowTitle;
        int windowWidth;
        int windowHeight;
    };


    class PlatformContext: public idk::Immobile
    {
    public:
        PlatformContext(const PlatformConfig &cfg);
        ~PlatformContext();

        bool running();
        void shutdown();
        void update();
        void processEvent(const void*);

        template <typename FeatureType, typename... Args>
        void addFeature(Args&&... args)
        {
            mFeatures.push(idk::New<FeatureType>(args...));
        }

        template <typename FeatureType>
        FeatureType *getFeature() noexcept
        {
            for (IPlatformFeature *feature: mFeatures)
            {
                if (FeatureType *ptr = dynamic_cast<FeatureType*>(feature))
                {
                    return ptr;
                }
            }
            return nullptr;
        }

    private:
        std::atomic<bool>  mRunning {true};
        std::atomic<bool>  mShutdown {false};
        idk::InplaceList<IPlatformFeature*, 16> mFeatures;

        void update_features();
        void kill_features();

    };

}
