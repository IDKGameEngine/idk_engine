#include "idk/gfx/RenderEngine.hpp"

#include "idk/platform/PlatformContext.hpp"
#include "idk/platform/VideoManager.hpp"

#include "libidk/Assert.hpp"
#include "libidk/log.hpp"

#include <SDL3/SDL_vulkan.h>

#include <vector>



idk::gfx::RenderEngine::RenderEngine(idk::PlatformContext &plat)
:   mWinHandle(plat.getService<VideoManager>()->getWindowHandle()),
    mInstance(VK_NULL_HANDLE),
    mDevice(VK_NULL_HANDLE),
    mSurface(VK_NULL_HANDLE)
{
    // Instance Creation
    // ---------------------------------------------------------------------------------------------
    VK_CHECK( volkInitialize() );

    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "How to Vulkan",
        .apiVersion = VK_API_VERSION_1_3
    };

    uint32_t instanceExtCount = 0;
    const char *const *instanceExtensions = SDL_Vulkan_GetInstanceExtensions(&instanceExtCount);
    VkInstanceCreateInfo instanceCI = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = instanceExtCount,
        .ppEnabledExtensionNames = instanceExtensions,
    };
    VK_CHECK( vkCreateInstance(&instanceCI, nullptr, &mInstance) );

    volkLoadInstance(mInstance);
    // ---------------------------------------------------------------------------------------------


    // Device Enumeration
    // ---------------------------------------------------------------------------------------------
    uint32_t deviceCount = 0;
    uint32_t deviceIndex = 0;
    VK_CHECK( vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr) );

    mDevices.resize(deviceCount);
    VK_CHECK( vkEnumeratePhysicalDevices(mInstance, &deviceCount, &mDevices[0]) );

    VkPhysicalDeviceProperties2 props = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2
    };
    vkGetPhysicalDeviceProperties2(mDevices[deviceIndex], &props);

    VLOG_INFO("[RenderEngine::RenderEngine] deviceCount: {}", deviceCount);
    VLOG_INFO("[RenderEngine::RenderEngine] deviceName:  {}", props.properties.deviceName);
    // ---------------------------------------------------------------------------------------------


    // Queue Creation
    // ---------------------------------------------------------------------------------------------
    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(mDevices[deviceIndex], &queueFamilyCount, nullptr);
    VLOG_INFO("queueFamilyCount: {}", queueFamilyCount);
    
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(mDevices[deviceIndex], &queueFamilyCount, queueFamilies.data());

    for (size_t i=0; i<queueFamilies.size(); i++)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            mQueueFamily = uint32_t(i);
            break;
        }
    }

    IDK_ASSERT(
        SDL_Vulkan_GetPresentationSupport(mInstance, mDevices[deviceIndex], mQueueFamily),
        "[RenderEngine::RenderEngine] Failure on SDL_Vulkan_GetPresentationSupport"
    );

    const float qfpriorities { 1.0f };
    VkDeviceQueueCreateInfo queueCI {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = mQueueFamily,
        .queueCount = 1,
        .pQueuePriorities = &qfpriorities
    };
    // ---------------------------------------------------------------------------------------------


    // Device Setup
    // ---------------------------------------------------------------------------------------------
    const std::vector<const char*> deviceExtensions { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    VkPhysicalDeviceVulkan12Features enabledVk12Features {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .descriptorIndexing = true,
        .shaderSampledImageArrayNonUniformIndexing = true,
        .descriptorBindingVariableDescriptorCount = true,
        .runtimeDescriptorArray = true,
        .bufferDeviceAddress = true
    };
    VkPhysicalDeviceVulkan13Features enabledVk13Features {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext = &enabledVk12Features,
        .synchronization2 = true,
        .dynamicRendering = true,
    };
    VkPhysicalDeviceFeatures enabledVk10Features {
        .samplerAnisotropy = VK_TRUE
    };
    VkDeviceCreateInfo deviceCI {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &enabledVk13Features,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queueCI,
        .enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
        .ppEnabledExtensionNames = deviceExtensions.data(),
        .pEnabledFeatures = &enabledVk10Features
    };
    VK_CHECK( vkCreateDevice(mDevices[deviceIndex], &deviceCI, nullptr, &mDevice) );
    volkLoadDevice(mDevice);
    vkGetDeviceQueue(mDevice, mQueueFamily, 0, &mGraphicsQueue);
    // ---------------------------------------------------------------------------------------------

    // VMA
    // ---------------------------------------------------------------------------------------------
    VmaVulkanFunctions vkFunctions {
        .vkGetInstanceProcAddr = vkGetInstanceProcAddr,
        .vkGetDeviceProcAddr = vkGetDeviceProcAddr,
        .vkCreateImage = vkCreateImage
    };
    VmaAllocatorCreateInfo allocatorCI {
        .flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT, 
        .physicalDevice = mDevices[deviceIndex],
        .device = mDevice,
        .pVulkanFunctions = &vkFunctions,
        .instance = mInstance
    };
    VK_CHECK( vmaCreateAllocator(&allocatorCI, &mAllocator) );
    // ---------------------------------------------------------------------------------------------


    // Create Surface
    // ---------------------------------------------------------------------------------------------
    IDK_ASSERT(
        SDL_Vulkan_CreateSurface((SDL_Window*)mWinHandle, mInstance, nullptr, &mSurface),
        "[RenderEngine::RenderEngine] Failure on SDL_Vulkan_CreateSurface"
    );
    VkSurfaceCapabilitiesKHR surfaceCaps {  };
    VK_CHECK( vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mDevices[deviceIndex], mSurface, &surfaceCaps) );
    // ---------------------------------------------------------------------------------------------


    // Swapchain
    // ---------------------------------------------------------------------------------------------
    int winWidth = 0; int winHeight = 0;
    SDL_GetWindowSize((SDL_Window*)mWinHandle, &winWidth, &winHeight);

    VkExtent2D swapchainExtent{ surfaceCaps.currentExtent };
    if (surfaceCaps.currentExtent.width == 0xFFFFFFFF)
    {
        swapchainExtent = { .width = static_cast<uint32_t>(winWidth), .height = static_cast<uint32_t>(winHeight) };
    }

    const VkFormat imageFormat{ VK_FORMAT_B8G8R8A8_SRGB };
    VkSwapchainCreateInfoKHR swapchainCI {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = mSurface,
        .minImageCount = surfaceCaps.minImageCount,
        .imageFormat = imageFormat,
        .imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
        .imageExtent{.width = swapchainExtent.width, .height = swapchainExtent.height },
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR
    };
    VK_CHECK( vkCreateSwapchainKHR(mDevice, &swapchainCI, nullptr, &mSwapchain) );

    uint32_t imageCount { 0 };
    VK_CHECK( vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount, nullptr) );

    mSwapchainImages.resize(imageCount);
    VK_CHECK( vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount, &mSwapchainImages[0]) );
    // swapchainImageViews.resize(imageCount);
    // ---------------------------------------------------------------------------------------------


    // Depth Attachment
    // ---------------------------------------------------------------------------------------------
    std::vector<VkFormat> depthFormatList{ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
    VkFormat depthFormat{ VK_FORMAT_UNDEFINED };
    for (VkFormat &format: depthFormatList)
    {
        VkFormatProperties2 formatProperties{ .sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2 };
        vkGetPhysicalDeviceFormatProperties2(mDevices[deviceIndex], format, &formatProperties);

        if (formatProperties.formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            depthFormat = format;
            break;
        }
    }
    VkImageCreateInfo depthImageCI {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = depthFormat,
        .extent{.width = static_cast<uint32_t>(winWidth), .height = static_cast<uint32_t>(winHeight), .depth = 1 },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };
    VmaAllocationCreateInfo allocCI {
        .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO
    };
    VK_CHECK( vmaCreateImage(mAllocator, &depthImageCI, &allocCI, &mDepthImage, &mDepthImageAllocation, nullptr) );

    VkImageViewCreateInfo depthViewCI{ 
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = mDepthImage,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = depthFormat,
        .subresourceRange{ .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT, .levelCount = 1, .layerCount = 1 }
    };
    VK_CHECK( vkCreateImageView(mDevice, &depthViewCI, nullptr, &mDepthImageView) );
    // ---------------------------------------------------------------------------------------------


    mSwapchainExtent = swapchainExtent;
    mSwapchainImageFormat = imageFormat;

    // Swapchain image views
    mSwapchainImageViews.resize(imageCount);
    for (uint32_t i = 0; i < imageCount; ++i)
    {
        VkImageViewCreateInfo viewCI {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = mSwapchainImages[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = imageFormat,
            .subresourceRange {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };

        VK_CHECK( vkCreateImageView(mDevice, &viewCI, nullptr, &mSwapchainImageViews[i]) );
    }

    // Render pass
    VkAttachmentDescription colorAttachment {
        .format = imageFormat,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    VkAttachmentReference colorAttachmentRef {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkAttachmentDescription depthAttachment {
        .format = depthFormat,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    VkAttachmentReference depthAttachmentRef {
        .attachment = 1,
        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    VkSubpassDescription subpass {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentRef,
        .pDepthStencilAttachment = &depthAttachmentRef
    };

    VkSubpassDependency dependency {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
    };

    std::array<VkAttachmentDescription, 2> attachments = {
        colorAttachment,
        depthAttachment
    };

    VkRenderPassCreateInfo renderPassCI {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = static_cast<uint32_t>(attachments.size()),
        .pAttachments = attachments.data(),
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dependency
    };

    VK_CHECK( vkCreateRenderPass(mDevice, &renderPassCI, nullptr, &mRenderPass) );

    // Framebuffers
    mFramebuffers.resize(imageCount);
    for (uint32_t i = 0; i < imageCount; ++i)
    {
        std::array<VkImageView, 2> attachments = {
            mSwapchainImageViews[i],
            mDepthImageView
        };

        VkFramebufferCreateInfo fbCI {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = mRenderPass,
            .attachmentCount = static_cast<uint32_t>(attachments.size()),
            .pAttachments = attachments.data(),
            .width = mSwapchainExtent.width,
            .height = mSwapchainExtent.height,
            .layers = 1
        };

        VK_CHECK( vkCreateFramebuffer(mDevice, &fbCI, nullptr, &mFramebuffers[i]) );
    }

    // Per-frame command pools / semaphores / fences
    for (uint32_t i = 0; i < NUM_FRAMES_IN_FLIGHT; ++i)
    {
        FrameData &frame = mFrames[i];

        VkCommandPoolCreateInfo poolCI {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = mQueueFamily
        };
        VK_CHECK( vkCreateCommandPool(mDevice, &poolCI, nullptr, &frame.commandPool) );

        VkCommandBufferAllocateInfo allocCI {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = frame.commandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1
        };
        VK_CHECK( vkAllocateCommandBuffers(mDevice, &allocCI, &frame.commandBuffer) );

        VkSemaphoreCreateInfo semCI {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
        };
        VK_CHECK( vkCreateSemaphore(mDevice, &semCI, nullptr, &frame.imageAvailable) );
        VK_CHECK( vkCreateSemaphore(mDevice, &semCI, nullptr, &frame.renderFinished) );

        VkFenceCreateInfo fenceCI {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT
        };
        VK_CHECK( vkCreateFence(mDevice, &fenceCI, nullptr, &frame.inFlightFence) );
    }
}


idk::gfx::RenderEngine::~RenderEngine()
{

}


void idk::gfx::RenderEngine::update()
{
    FrameData &frame = mFrames[mFrameIdx];

    VK_CHECK( vkWaitForFences(mDevice, 1, &frame.inFlightFence, VK_TRUE, UINT64_MAX) );
    VK_CHECK( vkResetFences(mDevice, 1, &frame.inFlightFence) );

    uint32_t imageIndex = 0;
    VkResult acquireResult = vkAcquireNextImageKHR(
        mDevice,
        mSwapchain,
        UINT64_MAX,
        frame.imageAvailable,
        VK_NULL_HANDLE,
        &imageIndex
    );

    if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR || acquireResult == VK_SUBOPTIMAL_KHR)
    {
        return;
    }

    VK_CHECK( acquireResult );

    VK_CHECK( vkResetCommandPool(mDevice, frame.commandPool, 0) );

    VkCommandBufferBeginInfo beginInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };
    VK_CHECK( vkBeginCommandBuffer(frame.commandBuffer, &beginInfo) );

    VkClearValue clearValues[2] = {};
    clearValues[0].color.float32[0] = 0.22f;
    clearValues[0].color.float32[1] = 0.42f;
    clearValues[0].color.float32[2] = 0.54f;
    clearValues[0].color.float32[3] = 1.0f;
    clearValues[1].depthStencil = { .depth = 1.0f, .stencil = 0 };

    VkRenderPassBeginInfo renderPassBegin {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = mRenderPass,
        .framebuffer = mFramebuffers[imageIndex],
        .renderArea = { { 0, 0 }, mSwapchainExtent },
        .clearValueCount = 2,
        .pClearValues = clearValues
    };

    vkCmdBeginRenderPass(frame.commandBuffer, &renderPassBegin, VK_SUBPASS_CONTENTS_INLINE);

    // placeholder draw call
    // vkCmdDraw(...)

    vkCmdEndRenderPass(frame.commandBuffer);
    VK_CHECK( vkEndCommandBuffer(frame.commandBuffer) );

    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &frame.imageAvailable,
        .pWaitDstStageMask = &waitStage,
        .commandBufferCount = 1,
        .pCommandBuffers = &frame.commandBuffer,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &frame.renderFinished
    };

    VK_CHECK( vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, frame.inFlightFence) );

    VkPresentInfoKHR presentInfo {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &frame.renderFinished,
        .swapchainCount = 1,
        .pSwapchains = &mSwapchain,
        .pImageIndices = &imageIndex
    };

    VkResult presentResult = vkQueuePresentKHR(mGraphicsQueue, &presentInfo);
    if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR)
    {
        return;
    }

    VK_CHECK( presentResult );

    mFrameIdx = (mFrameIdx + 1U) % NUM_FRAMES_IN_FLIGHT;
}


void idk::gfx::RenderEngine::shutdown()
{
    if (mDevice != VK_NULL_HANDLE)
    {
        vkDeviceWaitIdle(mDevice);

        for (uint32_t i=0; i<NUM_FRAMES_IN_FLIGHT; ++i)
        {
            FrameData &frame = mFrames[i];
            if (frame.renderFinished != VK_NULL_HANDLE)
            {
                vkDestroySemaphore(mDevice, frame.renderFinished, nullptr);
            }
            if (frame.imageAvailable != VK_NULL_HANDLE)
            {
                vkDestroySemaphore(mDevice, frame.imageAvailable, nullptr);
            }
            if (frame.inFlightFence != VK_NULL_HANDLE)
            {
                vkDestroyFence(mDevice, frame.inFlightFence, nullptr);
            }
            if (frame.commandPool != VK_NULL_HANDLE)
            {
                vkDestroyCommandPool(mDevice, frame.commandPool, nullptr);
            }
        }

        for (uint32_t i=0; i<mSwapchainImageViews.size(); ++i)
        {
            if (mSwapchainImageViews[i] != VK_NULL_HANDLE)
            {
                vkDestroyImageView(mDevice, mSwapchainImageViews[i], nullptr);
            }
        }

        for (uint32_t i=0; i<mFramebuffers.size(); ++i)
        {
            if (mFramebuffers[i] != VK_NULL_HANDLE)
            {
                vkDestroyFramebuffer(mDevice, mFramebuffers[i], nullptr);
            }
        }

        if (mDepthImageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(mDevice, mDepthImageView, nullptr);
        }
        if (mDepthImage != VK_NULL_HANDLE && mAllocator != VK_NULL_HANDLE)
        {
            vmaDestroyImage(mAllocator, mDepthImage, mDepthImageAllocation);
        }
        if (mRenderPass != VK_NULL_HANDLE)
        {
            vkDestroyRenderPass(mDevice, mRenderPass, nullptr);
        }
        if (mSwapchain != VK_NULL_HANDLE)
        {
            vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);
        }
        if (mSurface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
        }
        if (mDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(mDevice, nullptr);
        }
        if (mAllocator != VK_NULL_HANDLE)
        {
            vmaDestroyAllocator(mAllocator);
        }
        if (mInstance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(mInstance, nullptr);
        }
    }
}

