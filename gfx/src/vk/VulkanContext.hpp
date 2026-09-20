#pragma once

#include "libidk/gpu/vk.hpp"
#include "libidk/dsa/List.hpp"
#include <vector>


namespace idk::gfx
{
    class VulkanContext
    {
    public:
        VulkanContext(void *win);
        ~VulkanContext();

    private:
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

        void init_vulkan();
        void init_swapchain();
        void init_commands();
        void init_sync_structures();

        void create_swapchain(uint32_t w, uint32_t h);
        void destroy_swapchain();

    };
}
