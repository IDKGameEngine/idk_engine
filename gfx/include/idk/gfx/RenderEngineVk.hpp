#pragma once

// #include "libidk/gpu/vk.hpp"
// #include "libidk/dsa/List.hpp"
// #include <vector>

// namespace idk
// {
// class PlatformContext;
// }

// namespace idk::gfx
// {

// class RenderEngine
// {
// public:
//     explicit RenderEngine(PlatformContext& platform);
//     ~RenderEngine();

//     RenderEngine(const RenderEngine&) = delete;
//     RenderEngine& operator=(const RenderEngine&) = delete;

//     RenderEngine(RenderEngine&&) = delete;
//     RenderEngine& operator=(RenderEngine&&) = delete;

//     void update();
//     void shutdown();

// private:
//     struct FrameData
//     {
//         VkCommandPool   commandPool   = VK_NULL_HANDLE;
//         VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
//         VkSemaphore     imageAvailable = VK_NULL_HANDLE;
//         VkSemaphore     renderFinished = VK_NULL_HANDLE;
//         VkFence          inFlightFence = VK_NULL_HANDLE;
//     };

//     static constexpr uint32_t kFramesInFlight = 2U;
//     static constexpr uint32_t kMaxSwapchainImages = 16U;

//     void createInstance();
//     void createSurface();
//     void selectPhysicalDevice();
//     void createDevice();
//     void createAllocator();

//     void createSwapchain();
//     void destroySwapchain();
//     void recreateSwapchain();

//     void createDepthResources();
//     void destroyDepthResources();

//     void createFrameData();
//     void destroyFrameData();

//     void recordFrame(FrameData& frame, uint32_t imageIndex);
//     bool beginFrame(FrameData& frame, uint32_t& imageIndex);
//     void submitFrame(FrameData& frame);
//     void presentFrame(FrameData& frame, uint32_t imageIndex);

//     uint32_t findGraphicsQueueFamily(VkPhysicalDevice device) const;
//     VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) const;
//     VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& presentModes) const;
//     VkExtent2D chooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
//     VkFormat findDepthFormat() const;

//     void transitionImage(
//         VkCommandBuffer commandBuffer,
//         VkImage image,
//         VkImageLayout oldLayout,
//         VkImageLayout newLayout,
//         VkImageAspectFlags aspectMask);

// private:
//     bool mInitialized = false;

//     void* mWinHandle = nullptr;

//     VkInstance       mInstance       = VK_NULL_HANDLE;
//     VkSurfaceKHR     mSurface        = VK_NULL_HANDLE;
//     VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
//     VkDevice         mDevice         = VK_NULL_HANDLE;

//     VmaAllocator mAllocator = VK_NULL_HANDLE;

//     VkQueue  mGraphicsQueue = VK_NULL_HANDLE;
//     uint32_t mQueueFamily = VK_QUEUE_FAMILY_IGNORED;

//     VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;

//     VkFormat   mSwapchainImageFormat = VK_FORMAT_UNDEFINED;
//     VkExtent2D mSwapchainExtent{};

//     InplaceList<VkImage, kMaxSwapchainImages>     mSwapchainImages;
//     InplaceList<VkImageView, kMaxSwapchainImages> mSwapchainImageViews;
//     InplaceList<VkFence, kMaxSwapchainImages>     mImagesInFlight;

//     VkFormat          mDepthFormat = VK_FORMAT_UNDEFINED;
//     VkImage           mDepthImage = VK_NULL_HANDLE;
//     VkImageView       mDepthImageView = VK_NULL_HANDLE;
//     VmaAllocation     mDepthImageAllocation = VK_NULL_HANDLE;

//     FrameData mFrames[kFramesInFlight]{};
//     uint32_t mFrameIdx = 0;
// };

// } // namespace idk::gfx
