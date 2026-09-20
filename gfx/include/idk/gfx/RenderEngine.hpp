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

    InplaceList<VkPhysicalDevice, 16>   mPhysicalDevices;

    void                               *mWinHandle;
    VkInstance                          mInstance;
    VkPhysicalDevice                    mPhysicalDevice; // GPU chosen as the default device
    VkDevice                            mDevice;         // Vulkan device for commands
    VkSurfaceKHR                        mSurface;        // Vulkan window surface

    VkSwapchainKHR                      mSwapchain;
    VkFormat                            mSwapchainImageFormat;
    // std::vector<VkImage>                mSwapchainImages;
    // std::vector<VkImageView>            mSwapchainImageViews;
    // VkExtent2D                          mSwapchainExtent;

    // size_t                              mFrameNo = 0;
    // FrameData                           mFrames[NUM_FRAMES];

    VkQueue                             mGraphicsQueue;
    uint32_t                            mQueueFamily;

};


/*
#pragma once

#include "libidk/gpu/vk.hpp"
#include "libidk/dsa/List.hpp"
#include <vector>


namespace idk
{
    class PlatformContext;
}


namespace idk::gfx
{
    class RenderEngine
    {
    public:
        RenderEngine(PlatformContext&);
        ~RenderEngine();
        void update() {  };
        void shutdown() {  };

    private:
        struct FrameData
        {
            VkCommandPool commandPool;
            VkCommandBuffer mainCommandBuffer;
        };
        static constexpr size_t NUM_FRAMES = 2U;

        void                               *mWinHandle;
        VkInstance                          mInstance;
        VkDebugUtilsMessengerEXT            mDebugMessenger; // Vulkan debug output handle
        VkPhysicalDevice                    mPhysicalDevice; // GPU chosen as the default device
        VkDevice                            mDevice;         // Vulkan device for commands
        VkSurfaceKHR                        mSurface;        // Vulkan window surface

        VkSwapchainKHR                      mSwapchain;
        VkFormat                            mSwapchainImageFormat;
        std::vector<VkImage>                mSwapchainImages;
        std::vector<VkImageView>            mSwapchainImageViews;
        VkExtent2D                          mSwapchainExtent;

        size_t                              mFrameNo = 0;
        FrameData                           mFrames[NUM_FRAMES];

        VkQueue                             mGraphicsQueue;
        uint32_t                            mGraphicsQueueFamily;

        void init_vulkan();
        void init_swapchain();
        void init_commands();
        void init_sync_structures();

        void create_swapchain(uint32_t w, uint32_t h);
        void destroy_swapchain();

        FrameData &getCurrentFrame() { return mFrames[mFrameNo % NUM_FRAMES]; }

    };
}
*/
