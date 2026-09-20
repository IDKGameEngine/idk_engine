// #include "idk/gfx/RenderEngineVk.hpp"

// #include "idk/platform/PlatformContext.hpp"
// #include "idk/platform/VideoManager.hpp"

// #include <SDL3/SDL_vulkan.h>

// #include <algorithm>
// #include <cstring>
// #include <array>
// #include <limits>
// #include <stdexcept>

// namespace
// {
//     constexpr const char* kApplicationName = "How to Vulkan";

//     constexpr std::array<const char*, 1> kDeviceExtensions = {
//         VK_KHR_SWAPCHAIN_EXTENSION_NAME
//     };

//     constexpr std::array<VkFormat, 2> kDepthFormats = {
//         VK_FORMAT_D32_SFLOAT_S8_UINT,
//         VK_FORMAT_D24_UNORM_S8_UINT
//     };
// } // namespace


// idk::gfx::RenderEngine::RenderEngine(idk::PlatformContext& platform)
// {
//     mWinHandle = platform.getService<idk::VideoManager>()->getWindowHandle();

//     createInstance();
//     createSurface();
//     selectPhysicalDevice();
//     createDevice();
//     createAllocator();

//     createSwapchain();
//     createDepthResources();
//     createFrameData();

//     mInitialized = true;
// }

// idk::gfx::RenderEngine::~RenderEngine()
// {
//     shutdown();
// }

// void idk::gfx::RenderEngine::createInstance()
// {
//     VK_CHECK( volkInitialize() );

//     VkApplicationInfo appInfo {
//         .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
//         .pApplicationName = kApplicationName,
//         .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
//         .pEngineName = "idk",
//         .engineVersion = VK_MAKE_VERSION(1, 0, 0),
//         .apiVersion = VK_API_VERSION_1_3,
//     };

//     uint32_t extensionCount = 0;
//     const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);
//     IDK_ASSERT(extensions!=nullptr, "[RenderEngine] SDL_Vulkan_GetInstanceExtensions failed");

//     VkInstanceCreateInfo createInfo {
//         .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
//         .pApplicationInfo = &appInfo,
//         .enabledExtensionCount = extensionCount,
//         .ppEnabledExtensionNames = extensions,
//     };

//     VK_CHECK( vkCreateInstance(&createInfo, nullptr, &mInstance) );
//     volkLoadInstance(mInstance);

//     VLOG_INFO("[RenderEngine] Vulkan instance created");
// }

// void idk::gfx::RenderEngine::createSurface()
// {
//     IDK_ASSERT(
//         SDL_Vulkan_CreateSurface( static_cast<SDL_Window*>(mWinHandle), mInstance, nullptr, &mSurface),
//         "[RenderEngine] SDL_Vulkan_CreateSurface failed"
//     );
// }

// uint32_t idk::gfx::RenderEngine::findGraphicsQueueFamily(
//     VkPhysicalDevice device) const
// {
//     uint32_t queueFamilyCount = 0;
//     vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

//     std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//     vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

//     for (uint32_t i = 0; i < queueFamilyCount; ++i)
//     {
//         if ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)
//         {
//             continue;
//         }

//         VkBool32 presentSupported = VK_FALSE;
//         VK_CHECK( vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mSurface, &presentSupported) );

//         if (presentSupported == VK_TRUE)
//         {
//             return i;
//         }
//     }

//     return VK_QUEUE_FAMILY_IGNORED;
// }

// void idk::gfx::RenderEngine::selectPhysicalDevice()
// {
//     uint32_t deviceCount = 0;
//     VK_CHECK( vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr) );
//     IDK_ASSERT(deviceCount > 0, "[RenderEngine] No Vulkan physical devices found");
//     std::vector<VkPhysicalDevice> devices(deviceCount);
//     VK_CHECK( vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data()) );

//     for (VkPhysicalDevice device : devices)
//     {
//         const uint32_t queueFamily = findGraphicsQueueFamily(device);
//         if (queueFamily == VK_QUEUE_FAMILY_IGNORED)
//         {
//             continue;
//         }

//         uint32_t extensionCount = 0;
//         VK_CHECK( vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr) );
//         std::vector<VkExtensionProperties> extensions(extensionCount);
//         VK_CHECK( vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data()) );

//         bool hasSwapchainExtension = false;
//         for (const VkExtensionProperties& extension : extensions)
//         {
//             if (std::strcmp(extension.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
//             {
//                 hasSwapchainExtension = true;
//                 break;
//             }
//         }

//         if (!hasSwapchainExtension)
//         {
//             continue;
//         }

//         VkPhysicalDeviceFeatures features {  };
//         vkGetPhysicalDeviceFeatures(device, &features);
//         if (features.samplerAnisotropy != VK_TRUE)
//         {
//             continue;
//         }

//         VkPhysicalDeviceVulkan12Features features12 {
//             .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
//         };

//         VkPhysicalDeviceVulkan13Features features13 {
//             .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
//             .pNext = &features12,
//         };

//         VkPhysicalDeviceFeatures2 features2 {
//             .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
//             .pNext = &features13,
//         };

//         vkGetPhysicalDeviceFeatures2(device, &features2);

//         if (!features12.descriptorIndexing ||
//             !features12.shaderSampledImageArrayNonUniformIndexing ||
//             !features12.descriptorBindingVariableDescriptorCount ||
//             !features12.runtimeDescriptorArray ||
//             !features12.bufferDeviceAddress ||
//             !features13.synchronization2 ||
//             !features13.dynamicRendering)
//         {
//             continue;
//         }

//         mPhysicalDevice = device;
//         mQueueFamily = queueFamily;

//         VkPhysicalDeviceProperties properties {  };
//         vkGetPhysicalDeviceProperties(device, &properties);
//         VLOG_INFO("[RenderEngine] Selected Vulkan device: {}", properties.deviceName);

//         return;
//     }

//     IDK_ASSERT(false, "[RenderEngine] No suitable Vulkan physical device found");
// }

// void idk::gfx::RenderEngine::createDevice()
// {
//     constexpr float queuePriority = 1.0f;

//     VkDeviceQueueCreateInfo queueInfo{
//         .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
//         .queueFamilyIndex = mQueueFamily,
//         .queueCount = 1,
//         .pQueuePriorities = &queuePriority,
//     };

//     VkPhysicalDeviceVulkan12Features features12{
//         .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
//         .descriptorIndexing = VK_TRUE,
//         .shaderSampledImageArrayNonUniformIndexing = VK_TRUE,
//         .descriptorBindingVariableDescriptorCount = VK_TRUE,
//         .runtimeDescriptorArray = VK_TRUE,
//         .bufferDeviceAddress = VK_TRUE,
//     };

//     VkPhysicalDeviceVulkan13Features features13{
//         .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
//         .pNext = &features12,
//         .synchronization2 = VK_TRUE,
//         .dynamicRendering = VK_TRUE,
//     };

//     VkPhysicalDeviceFeatures features10{
//         .samplerAnisotropy = VK_TRUE,
//     };

//     VkDeviceCreateInfo createInfo{
//         .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
//         .pNext = &features13,
//         .queueCreateInfoCount = 1,
//         .pQueueCreateInfos = &queueInfo,
//         .enabledExtensionCount =
//             static_cast<uint32_t>(kDeviceExtensions.size()),
//         .ppEnabledExtensionNames = kDeviceExtensions.data(),
//         .pEnabledFeatures = &features10,
//     };

//     VK_CHECK( vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice) );
//     volkLoadDevice(mDevice);
//     vkGetDeviceQueue(mDevice, mQueueFamily, 0, &mGraphicsQueue);
// }

// void idk::gfx::RenderEngine::createAllocator()
// {
//     VmaVulkanFunctions functions{
//         .vkGetInstanceProcAddr = vkGetInstanceProcAddr,
//         .vkGetDeviceProcAddr = vkGetDeviceProcAddr,
//         .vkCreateImage = vkCreateImage,
//     };

//     VmaAllocatorCreateInfo createInfo{
//         .flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
//         .physicalDevice = mPhysicalDevice,
//         .device = mDevice,
//         .pVulkanFunctions = &functions,
//         .instance = mInstance,
//         .vulkanApiVersion = VK_API_VERSION_1_3,
//     };

//     VK_CHECK(vmaCreateAllocator(&createInfo, &mAllocator));
// }

// VkSurfaceFormatKHR idk::gfx::RenderEngine::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) const
// {
//     for (const VkSurfaceFormatKHR& format : formats)
//     {
//         if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
//             format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
//         {
//             return format;
//         }
//     }

//     IDK_ASSERT(
//         !formats.empty(),
//         "[RenderEngine] Surface reports no formats");

//     return formats[0];
// }

// VkPresentModeKHR idk::gfx::RenderEngine::choosePresentMode(const std::vector<VkPresentModeKHR>& presentModes) const
// {
//     for (VkPresentModeKHR mode : presentModes)
//     {
//         if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
//             return mode;
//     }

//     return VK_PRESENT_MODE_FIFO_KHR;
// }

// VkExtent2D idk::gfx::RenderEngine::chooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
// {
//     if (capabilities.currentExtent.width !=
//         std::numeric_limits<uint32_t>::max())
//     {
//         return capabilities.currentExtent;
//     }

//     int width = 0;
//     int height = 0;
//     SDL_GetWindowSizeInPixels(static_cast<SDL_Window*>(mWinHandle), &width, &height);

//     VkExtent2D extent {
//         .width = static_cast<uint32_t>(std::max(width, 0)),
//         .height = static_cast<uint32_t>(std::max(height, 0)),
//     };
//     extent.width = std::clamp(
//         extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width
//     );
//     extent.height = std::clamp(
//         extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height
//     );

//     return extent;
// }

// void idk::gfx::RenderEngine::createSwapchain()
// {
//     VkSurfaceCapabilitiesKHR capabilities{};
//     VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
//         mPhysicalDevice,
//         mSurface,
//         &capabilities));

//     uint32_t formatCount = 0;
//     VK_CHECK( vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, mSurface, &formatCount, nullptr) );

//     std::vector<VkSurfaceFormatKHR> formats(formatCount);
//     VK_CHECK( vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, mSurface, &formatCount, formats.data()) );

//     uint32_t presentModeCount = 0;
//     VK_CHECK( vkGetPhysicalDeviceSurfacePresentModesKHR(mPhysicalDevice, mSurface, &presentModeCount, nullptr) );

//     std::vector<VkPresentModeKHR> presentModes(presentModeCount);
//     VK_CHECK( vkGetPhysicalDeviceSurfacePresentModesKHR(mPhysicalDevice, mSurface, &presentModeCount, presentModes.data()) );

//     const VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(formats);
//     const VkPresentModeKHR presentMode = choosePresentMode(presentModes);
//     const VkExtent2D extent = chooseSwapchainExtent(capabilities);

//     IDK_ASSERT(
//         (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) != 0,
//         "[RenderEngine] Surface does not support color attachments"
//     );

//     uint32_t imageCount = capabilities.minImageCount + 1;
//     if (capabilities.maxImageCount > 0)
//     {
//         imageCount = std::min(imageCount, capabilities.maxImageCount);
//     }

//     IDK_ASSERT(
//         imageCount <= kMaxSwapchainImages,
//         "[RenderEngine] Swapchain has more images than InplaceList capacity"
//     );

//     VkSwapchainCreateInfoKHR createInfo {
//         .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
//         .surface = mSurface,
//         .minImageCount = imageCount,
//         .imageFormat = surfaceFormat.format,
//         .imageColorSpace = surfaceFormat.colorSpace,
//         .imageExtent = extent,
//         .imageArrayLayers = 1,
//         .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
//         .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
//         .preTransform = capabilities.currentTransform,
//         .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
//         .presentMode = presentMode,
//         .clipped = VK_TRUE,
//     };

//     VK_CHECK( vkCreateSwapchainKHR(mDevice, &createInfo, nullptr, &mSwapchain) );

//     uint32_t actualImageCount = 0;
//     VK_CHECK( vkGetSwapchainImagesKHR(mDevice, mSwapchain, &actualImageCount, nullptr) );

//     IDK_ASSERT(
//         actualImageCount <= kMaxSwapchainImages,
//         "[RenderEngine] Swapchain returned too many images"
//     );

//     mSwapchainImages.resize(actualImageCount);
//     VK_CHECK( vkGetSwapchainImagesKHR(mDevice, mSwapchain, &actualImageCount, &mSwapchainImages[0]) );
//     mSwapchainImageFormat = surfaceFormat.format;
//     mSwapchainExtent = extent;
//     mSwapchainImageViews.resize(actualImageCount);

//     for (uint32_t i = 0; i < actualImageCount; ++i)
//     {
//         VkImageViewCreateInfo viewInfo{
//             .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
//             .image = mSwapchainImages[i],
//             .viewType = VK_IMAGE_VIEW_TYPE_2D,
//             .format = mSwapchainImageFormat,
//             .subresourceRange{
//                 .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
//                 .baseMipLevel = 0,
//                 .levelCount = 1,
//                 .baseArrayLayer = 0,
//                 .layerCount = 1,
//             },
//         };

//         VK_CHECK(vkCreateImageView(
//             mDevice,
//             &viewInfo,
//             nullptr,
//             &mSwapchainImageViews[i]));
//     }

//     mImagesInFlight.resize(actualImageCount);

//     for (uint32_t i = 0; i < actualImageCount; ++i)
//         mImagesInFlight[i] = VK_NULL_HANDLE;
// }

// void idk::gfx::RenderEngine::destroySwapchain()
// {
//     for (uint32_t i = 0; i < mSwapchainImageViews.size(); ++i)
//     {
//         if (mSwapchainImageViews[i] != VK_NULL_HANDLE)
//         {
//             vkDestroyImageView(
//                 mDevice,
//                 mSwapchainImageViews[i],
//                 nullptr);

//             mSwapchainImageViews[i] = VK_NULL_HANDLE;
//         }
//     }

//     mSwapchainImageViews.resize(0);
//     mSwapchainImages.resize(0);
//     mImagesInFlight.resize(0);

//     if (mSwapchain != VK_NULL_HANDLE)
//     {
//         vkDestroySwapchainKHR(
//             mDevice,
//             mSwapchain,
//             nullptr);

//         mSwapchain = VK_NULL_HANDLE;
//     }
// }

// VkFormat idk::gfx::RenderEngine::findDepthFormat() const
// {
//     for (VkFormat format : kDepthFormats)
//     {
//         VkFormatProperties properties{};
//         vkGetPhysicalDeviceFormatProperties(
//             mPhysicalDevice,
//             format,
//             &properties);

//         if ((properties.optimalTilingFeatures &
//              VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) != 0)
//         {
//             return format;
//         }
//     }

//     return VK_FORMAT_UNDEFINED;
// }

// void idk::gfx::RenderEngine::createDepthResources()
// {
//     mDepthFormat = findDepthFormat();

//     IDK_ASSERT(
//         mDepthFormat != VK_FORMAT_UNDEFINED,
//         "[RenderEngine] No suitable depth format found");

//     VkImageCreateInfo imageInfo{
//         .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
//         .imageType = VK_IMAGE_TYPE_2D,
//         .format = mDepthFormat,
//         .extent{
//             .width = mSwapchainExtent.width,
//             .height = mSwapchainExtent.height,
//             .depth = 1,
//         },
//         .mipLevels = 1,
//         .arrayLayers = 1,
//         .samples = VK_SAMPLE_COUNT_1_BIT,
//         .tiling = VK_IMAGE_TILING_OPTIMAL,
//         .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
//         .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
//         .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
//     };

//     VmaAllocationCreateInfo allocationInfo{
//         .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
//         .usage = VMA_MEMORY_USAGE_AUTO,
//     };

//     VK_CHECK(vmaCreateImage(
//         mAllocator,
//         &imageInfo,
//         &allocationInfo,
//         &mDepthImage,
//         &mDepthImageAllocation,
//         nullptr));

//     VkImageViewCreateInfo viewInfo{
//         .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
//         .image = mDepthImage,
//         .viewType = VK_IMAGE_VIEW_TYPE_2D,
//         .format = mDepthFormat,
//         .subresourceRange{
//             .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
//             .baseMipLevel = 0,
//             .levelCount = 1,
//             .baseArrayLayer = 0,
//             .layerCount = 1,
//         },
//     };

//     VK_CHECK(vkCreateImageView(
//         mDevice,
//         &viewInfo,
//         nullptr,
//         &mDepthImageView));

//     // The first rendering transition is performed by recordFrame().
// }

// void idk::gfx::RenderEngine::destroyDepthResources()
// {
//     if (mDepthImageView != VK_NULL_HANDLE)
//     {
//         vkDestroyImageView(
//             mDevice,
//             mDepthImageView,
//             nullptr);

//         mDepthImageView = VK_NULL_HANDLE;
//     }

//     if (mDepthImage != VK_NULL_HANDLE)
//     {
//         vmaDestroyImage(
//             mAllocator,
//             mDepthImage,
//             mDepthImageAllocation);

//         mDepthImage = VK_NULL_HANDLE;
//         mDepthImageAllocation = VK_NULL_HANDLE;
//     }

//     mDepthFormat = VK_FORMAT_UNDEFINED;
// }

// void idk::gfx::RenderEngine::createFrameData()
// {
//     for (uint32_t i = 0; i < kFramesInFlight; ++i)
//     {
//         FrameData& frame = mFrames[i];

//         VkCommandPoolCreateInfo poolInfo{
//             .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
//             .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
//             .queueFamilyIndex = mQueueFamily,
//         };

//         VK_CHECK(vkCreateCommandPool(
//             mDevice,
//             &poolInfo,
//             nullptr,
//             &frame.commandPool));

//         VkCommandBufferAllocateInfo allocationInfo{
//             .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
//             .commandPool = frame.commandPool,
//             .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
//             .commandBufferCount = 1,
//         };

//         VK_CHECK(vkAllocateCommandBuffers(
//             mDevice,
//             &allocationInfo,
//             &frame.commandBuffer));

//         VkSemaphoreCreateInfo semaphoreInfo{
//             .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
//         };

//         VK_CHECK(vkCreateSemaphore(
//             mDevice,
//             &semaphoreInfo,
//             nullptr,
//             &frame.imageAvailable));

//         VK_CHECK(vkCreateSemaphore(
//             mDevice,
//             &semaphoreInfo,
//             nullptr,
//             &frame.renderFinished));

//         VkFenceCreateInfo fenceInfo{
//             .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
//             .flags = VK_FENCE_CREATE_SIGNALED_BIT,
//         };

//         VK_CHECK(vkCreateFence(
//             mDevice,
//             &fenceInfo,
//             nullptr,
//             &frame.inFlightFence));
//     }
// }

// void idk::gfx::RenderEngine::destroyFrameData()
// {
//     if (mDevice == VK_NULL_HANDLE)
//         return;

//     for (uint32_t i = 0; i < kFramesInFlight; ++i)
//     {
//         FrameData& frame = mFrames[i];

//         if (frame.renderFinished != VK_NULL_HANDLE)
//         {
//             vkDestroySemaphore(
//                 mDevice,
//                 frame.renderFinished,
//                 nullptr);

//             frame.renderFinished = VK_NULL_HANDLE;
//         }

//         if (frame.imageAvailable != VK_NULL_HANDLE)
//         {
//             vkDestroySemaphore(
//                 mDevice,
//                 frame.imageAvailable,
//                 nullptr);

//             frame.imageAvailable = VK_NULL_HANDLE;
//         }

//         if (frame.inFlightFence != VK_NULL_HANDLE)
//         {
//             vkDestroyFence(
//                 mDevice,
//                 frame.inFlightFence,
//                 nullptr);

//             frame.inFlightFence = VK_NULL_HANDLE;
//         }

//         if (frame.commandPool != VK_NULL_HANDLE)
//         {
//             vkDestroyCommandPool(
//                 mDevice,
//                 frame.commandPool,
//                 nullptr);

//             frame.commandPool = VK_NULL_HANDLE;
//             frame.commandBuffer = VK_NULL_HANDLE;
//         }
//     }
// }

// void idk::gfx::RenderEngine::transitionImage(
//     VkCommandBuffer commandBuffer,
//     VkImage image,
//     VkImageLayout oldLayout,
//     VkImageLayout newLayout,
//     VkImageAspectFlags aspectMask)
// {
//     VkImageMemoryBarrier2 barrier{
//         .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
//         .srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
//         .srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT,
//         .dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
//         .dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT |
//                          VK_ACCESS_2_MEMORY_WRITE_BIT,
//         .oldLayout = oldLayout,
//         .newLayout = newLayout,
//         .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
//         .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
//         .image = image,
//         .subresourceRange{
//             .aspectMask = aspectMask,
//             .baseMipLevel = 0,
//             .levelCount = 1,
//             .baseArrayLayer = 0,
//             .layerCount = 1,
//         },
//     };

//     VkDependencyInfo dependencyInfo{
//         .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
//         .imageMemoryBarrierCount = 1,
//         .pImageMemoryBarriers = &barrier,
//     };

//     vkCmdPipelineBarrier2(
//         commandBuffer,
//         &dependencyInfo);
// }

// bool idk::gfx::RenderEngine::beginFrame(
//     FrameData& frame,
//     uint32_t& imageIndex)
// {
//     VK_CHECK(vkWaitForFences(
//         mDevice,
//         1,
//         &frame.inFlightFence,
//         VK_TRUE,
//         UINT64_MAX));

//     const VkResult acquireResult = vkAcquireNextImageKHR(
//         mDevice,
//         mSwapchain,
//         UINT64_MAX,
//         frame.imageAvailable,
//         VK_NULL_HANDLE,
//         &imageIndex);

//     if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
//     {
//         recreateSwapchain();
//         return false;
//     }

//     if (acquireResult != VK_SUCCESS &&
//         acquireResult != VK_SUBOPTIMAL_KHR)
//     {
//         VK_CHECK(acquireResult);
//     }

//     if (mImagesInFlight[imageIndex] != VK_NULL_HANDLE)
//     {
//         VK_CHECK(vkWaitForFences(
//             mDevice,
//             1,
//             &mImagesInFlight[imageIndex],
//             VK_TRUE,
//             UINT64_MAX));
//     }

//     mImagesInFlight[imageIndex] = frame.inFlightFence;

//     VK_CHECK(vkResetFences(
//         mDevice,
//         1,
//         &frame.inFlightFence));

//     VK_CHECK(vkResetCommandPool(
//         mDevice,
//         frame.commandPool,
//         0));

//     VkCommandBufferBeginInfo beginInfo{
//         .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
//         .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
//     };

//     VK_CHECK(vkBeginCommandBuffer(
//         frame.commandBuffer,
//         &beginInfo));

//     return true;
// }

// void idk::gfx::RenderEngine::recordFrame(
//     FrameData& frame,
//     uint32_t imageIndex)
// {
//     transitionImage(
//         frame.commandBuffer, mSwapchainImages[imageIndex], VK_IMAGE_LAYOUT_UNDEFINED,
//         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT
//     );

//     transitionImage(
//         frame.commandBuffer, mDepthImage, VK_IMAGE_LAYOUT_UNDEFINED,
//         VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL, VK_IMAGE_ASPECT_DEPTH_BIT
//     );

//     VkClearValue colorClear{};
//     colorClear.color.float32[0] = 0.22f;
//     colorClear.color.float32[1] = 0.42f;
//     colorClear.color.float32[2] = 0.54f;
//     colorClear.color.float32[3] = 1.0f;

//     VkClearValue depthClear {  };
//     depthClear.depthStencil = { .depth = 1.0f, .stencil = 0 };

//     VkRenderingAttachmentInfo colorAttachment {
//         .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
//         .imageView = mSwapchainImageViews[imageIndex],
//         .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
//         .resolveMode = VK_RESOLVE_MODE_NONE,
//         .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
//         .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
//         .clearValue = colorClear,
//     };

//     VkRenderingAttachmentInfo depthAttachment {
//         .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
//         .imageView = mDepthImageView,
//         .imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
//         .resolveMode = VK_RESOLVE_MODE_NONE,
//         .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
//         .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
//         .clearValue = depthClear,
//     };

//     VkRenderingInfo renderingInfo {
//         .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
//         .renderArea{
//             .offset{0, 0},
//             .extent = mSwapchainExtent,
//         },
//         .layerCount = 1,
//         .colorAttachmentCount = 1,
//         .pColorAttachments = &colorAttachment,
//         .pDepthAttachment = &depthAttachment,
//     };

//     vkCmdBeginRendering(frame.commandBuffer, &renderingInfo);

//     // Placeholder draw calls go here.
//     // vkCmdBindPipeline(...);
//     // vkCmdDraw(...);

//     vkCmdEndRendering(frame.commandBuffer);

//     transitionImage(
//         frame.commandBuffer, mSwapchainImages[imageIndex], VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
//         VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_ASPECT_COLOR_BIT
//     );

//     VK_CHECK( vkEndCommandBuffer(frame.commandBuffer) );
// }

// void idk::gfx::RenderEngine::submitFrame(FrameData& frame)
// {
//     VkCommandBufferSubmitInfo commandBufferInfo {
//         .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
//         .commandBuffer = frame.commandBuffer,
//         .deviceMask = 0,
//     };

//     VkSemaphoreSubmitInfo waitSemaphoreInfo {
//         .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
//         .semaphore = frame.imageAvailable,
//         .value = 0,
//         .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
//         .deviceIndex = 0,
//     };

//     VkSemaphoreSubmitInfo signalSemaphoreInfo {
//         .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
//         .semaphore = frame.renderFinished,
//         .value = 0,
//         .stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT,
//         .deviceIndex = 0,
//     };

//     VkSubmitInfo2 submitInfo {
//         .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
//         .waitSemaphoreInfoCount = 1,
//         .pWaitSemaphoreInfos = &waitSemaphoreInfo,
//         .commandBufferInfoCount = 1,
//         .pCommandBufferInfos = &commandBufferInfo,
//         .signalSemaphoreInfoCount = 1,
//         .pSignalSemaphoreInfos = &signalSemaphoreInfo,
//     };

//     VK_CHECK( vkQueueSubmit2(mGraphicsQueue, 1, &submitInfo, frame.inFlightFence) );
// }

// void idk::gfx::RenderEngine::presentFrame(FrameData &frame, uint32_t imageIndex)
// {
//     VkPresentInfoKHR presentInfo {
//         .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
//         .waitSemaphoreCount = 1,
//         .pWaitSemaphores = &frame.renderFinished,
//         .swapchainCount = 1,
//         .pSwapchains = &mSwapchain,
//         .pImageIndices = &imageIndex,
//     };

//     const VkResult result = vkQueuePresentKHR(mGraphicsQueue, &presentInfo);
//     if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
//     {
//         recreateSwapchain();
//         return;
//     }

//     VK_CHECK(result);
// }

// void idk::gfx::RenderEngine::update()
// {
//     if (!mInitialized)
//     {
//         return;
//     }

//     FrameData& frame = mFrames[mFrameIdx];
//     uint32_t imageIndex = 0;

//     if (!beginFrame(frame, imageIndex))
//     {
//         return;
//     }

//     recordFrame(frame, imageIndex);
//     submitFrame(frame);
//     presentFrame(frame, imageIndex);

//     mFrameIdx = (mFrameIdx + 1U) % kFramesInFlight;
// }

// void idk::gfx::RenderEngine::recreateSwapchain()
// {
//     if (mDevice == VK_NULL_HANDLE)
//     {
//         return;
//     }

//     int width = 0; int height = 0;
//     SDL_GetWindowSizeInPixels(static_cast<SDL_Window*>(mWinHandle), &width, &height);
//     if (width == 0 || height == 0)
//     {
//         return;
//     }

//     VK_CHECK( vkDeviceWaitIdle(mDevice) );

//     destroyDepthResources();
//     destroySwapchain();

//     createSwapchain();
//     createDepthResources();
// }

// void idk::gfx::RenderEngine::shutdown()
// {
//     if (!mInitialized && mInstance == VK_NULL_HANDLE && mDevice == VK_NULL_HANDLE)
//     {
//         return;
//     }

//     if (mDevice != VK_NULL_HANDLE)
//     {
//         VK_CHECK(vkDeviceWaitIdle(mDevice));

//         destroyFrameData();
//         destroyDepthResources();
//         destroySwapchain();

//         if (mAllocator != VK_NULL_HANDLE)
//         {
//             vmaDestroyAllocator(mAllocator);
//             mAllocator = VK_NULL_HANDLE;
//         }

//         vkDestroyDevice(mDevice, nullptr);
//         mDevice = VK_NULL_HANDLE;
//         mGraphicsQueue = VK_NULL_HANDLE;
//     }

//     if (mSurface != VK_NULL_HANDLE)
//     {
//         vkDestroySurfaceKHR(
//             mInstance,
//             mSurface,
//             nullptr);

//         mSurface = VK_NULL_HANDLE;
//     }

//     if (mInstance != VK_NULL_HANDLE)
//     {
//         vkDestroyInstance(
//             mInstance,
//             nullptr);

//         mInstance = VK_NULL_HANDLE;
//     }

//     mPhysicalDevice = VK_NULL_HANDLE;
//     mQueueFamily = VK_QUEUE_FAMILY_IGNORED;
//     mInitialized = false;
//     mFrameIdx = 0;
// }
