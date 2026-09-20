#pragma once

#include "libidk/gpu/vk.hpp"
#include "libidk/dsa/List.hpp"


namespace idk
{
    class PlatformContext;
}

namespace idk::gfx
{
    class RenderEngine;
}


class idk::gfx::RenderEngine
{
public:
    RenderEngine(PlatformContext&);
    ~RenderEngine();
    void update() {  };
    void shutdown() {  };

private:
    // struct FrameData
    // {
    //     VkCommandPool commandPool;
    //     VkCommandBuffer mainCommandBuffer;
    // };
    // static constexpr size_t NUM_FRAMES = 2U;

    InplaceList<VkPhysicalDevice, 16>   mDevices;

    void                               *mWinHandle;
    VkInstance                          mInstance;
    VkPhysicalDevice                    mPhysicalDevice;
    VkDevice                            mDevice;
    VkSurfaceKHR                        mSurface;

    VkSwapchainKHR                      mSwapchain;
    VkFormat                            mSwapchainImageFormat;
    InplaceList<VkImage, 16>            mSwapchainImages;

    VkImage                             mDepthImage;
    VkImageView                         mDepthImageView;
    // std::vector<VkImage>                mSwapchainImages;
    // std::vector<VkImageView>            mSwapchainImageViews;
    // VkExtent2D                          mSwapchainExtent;

    // size_t                              mFrameNo = 0;
    // FrameData                           mFrames[NUM_FRAMES];

    VkQueue                             mGraphicsQueue;
    uint32_t                            mQueueFamily;

};

