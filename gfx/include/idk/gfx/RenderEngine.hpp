#pragma once

#include "libidk/gpu/vk.hpp"
#include "libidk/dsa/List.hpp"
#include <vk_mem_alloc.h>


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
    void update();
    void shutdown();

private:
    struct FrameData
    {
        VkCommandPool   commandPool = VK_NULL_HANDLE;
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
        VkSemaphore     imageAvailable = VK_NULL_HANDLE;
        VkSemaphore     renderFinished = VK_NULL_HANDLE;
        VkFence         inFlightFence = VK_NULL_HANDLE;
    };
    static constexpr uint32_t NUM_FRAMES_IN_FLIGHT = 2U;

    FrameData                           mFrames[NUM_FRAMES_IN_FLIGHT];
    uint32_t                            mFrameIdx = 0;

    InplaceList<VkImageView, 16>        mSwapchainImageViews;
    InplaceList<VkFramebuffer, 16>      mFramebuffers;

    VkRenderPass                        mRenderPass = VK_NULL_HANDLE;
    VkExtent2D                          mSwapchainExtent{ 0, 0 };

    InplaceList<VkPhysicalDevice, 16>   mDevices;

    void                               *mWinHandle;
    VkInstance                          mInstance;
    VkPhysicalDevice                    mPhysicalDevice;
    VkDevice                            mDevice;
    VkSurfaceKHR                        mSurface;

    VkSwapchainKHR                      mSwapchain;
    VkFormat                            mSwapchainImageFormat;
    InplaceList<VkImage, 16>            mSwapchainImages;

    VmaAllocator                        mAllocator = VK_NULL_HANDLE;
    VmaAllocation                       mDepthImageAllocation = VK_NULL_HANDLE;

    VkImage                             mDepthImage;
    VkImageView                         mDepthImageView;
    // std::vector<VkImage>                mSwapchainImages;
    // std::vector<VkImageView>            mSwapchainImageViews;
    // VkExtent2D                          mSwapchainExtent;

    VkQueue                             mGraphicsQueue;
    uint32_t                            mQueueFamily;

};

