#include "idk/gfx/RenderEngine.hpp"

#include "idk/platform/PlatformContext.hpp"
#include "idk/platform/VideoManager.hpp"

#include "libidk/Assert.hpp"
#include "libidk/log.hpp"

#include <vk_mem_alloc.h>
#include <SDL3/SDL_vulkan.h>

#include <vector>


static VmaAllocator  mAllocator {  };
static VmaAllocation mDepthImageAllocation {  };


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

}


idk::gfx::RenderEngine::~RenderEngine()
{
    if (mInstance != VK_NULL_HANDLE)
    {

    }
}



/*

#include "idk/gfx/RenderEngine.hpp"

#include "idk/platform/PlatformContext.hpp"
#include "idk/platform/VideoManager.hpp"

#include "libidk/Assert.hpp"
#include "libidk/log.hpp"
#include "VkBootstrap.h"

#include <SDL3/SDL_vulkan.h>
#include <vector>

static constexpr bool bUseValidationLayers = false;


idk::gfx::RenderEngine::RenderEngine(idk::PlatformContext &plat)
:   mWinHandle(plat.getService<VideoManager>()->getWindowHandle()),
    mInstance(VK_NULL_HANDLE),
    mPhysicalDevice(VK_NULL_HANDLE),
    mDevice(VK_NULL_HANDLE),
    mSurface(VK_NULL_HANDLE)
{
	init_vulkan();
	init_swapchain();
	init_commands();
	init_sync_structures();
}


idk::gfx::RenderEngine::~RenderEngine()
{
    if (mInstance != VK_NULL_HANDLE)
    {
        vkDeviceWaitIdle(mDevice);
		for (size_t i=0; i<NUM_FRAMES; i++)
        {
			vkDestroyCommandPool(mDevice, mFrames[i].commandPool, nullptr);
		}

        vkDestroyInstance(mInstance, nullptr);
        mInstance = VK_NULL_HANDLE;

        destroy_swapchain();
    
        vkDestroyDevice(mDevice, nullptr);
        vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
        vkb::destroy_debug_utils_messenger(mInstance, mDebugMessenger);
    }
}


void idk::gfx::RenderEngine::init_vulkan()
{
	vkb::InstanceBuilder builder;

	// Make the vulkan instance, with basic debug features
	auto inst_ret = builder
        .set_app_name("Example Vulkan Application")
        .request_validation_layers(bUseValidationLayers)
        .use_default_debug_messenger()
        .require_api_version(1, 3, 0)
        .build();

	vkb::Instance vkb_inst = inst_ret.value();

	// Grab the instance 
	mInstance = vkb_inst.instance;
	mDebugMessenger = vkb_inst.debug_messenger;


	SDL_Vulkan_CreateSurface((SDL_Window*)mWinHandle, mInstance, NULL, &mSurface);

	// Vulkan 1.3 features
	VkPhysicalDeviceVulkan13Features features{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
	features.dynamicRendering = true;
	features.synchronization2 = true;

	// Use vkbootstrap to select a gpu. 
	// We want a gpu that can write to the SDL surface and supports vulkan 1.3 with the correct features
	vkb::PhysicalDeviceSelector selector{ vkb_inst };
	vkb::PhysicalDevice physicalDevice = selector
		.set_minimum_version(1, 3)
		.set_required_features_13(features)
		.set_surface(mSurface)
		.select()
		.value();

	// Create the final vulkan device
	vkb::DeviceBuilder deviceBuilder{ physicalDevice };

	vkb::Device vkbDevice = deviceBuilder.build().value();

	// Get the VkDevice handle used in the rest of a vulkan application
	mDevice = vkbDevice.device;
	mPhysicalDevice = physicalDevice.physical_device;

    // Use vkbootstrap to get a Graphics queue
	mGraphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
	mGraphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

}


void idk::gfx::RenderEngine::init_swapchain()
{
    int w=0, h=0;
    SDL_GetWindowSize((SDL_Window*)mWinHandle, &w, &h);
    create_swapchain(w, h);
}


void idk::gfx::RenderEngine::init_commands()
{

}


void idk::gfx::RenderEngine::init_sync_structures()
{

}


void idk::gfx::RenderEngine::create_swapchain(uint32_t w, uint32_t h)
{
	vkb::SwapchainBuilder swapchainBuilder { mPhysicalDevice, mDevice, mSurface };

	mSwapchainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

	vkb::Swapchain vkbSwapchain = swapchainBuilder
		//.use_default_format_selection()
		.set_desired_format(VkSurfaceFormatKHR{ .format = mSwapchainImageFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
		//use vsync present mode
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		.set_desired_extent(w, h)
		.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		.build()
		.value();

    mSwapchainExtent = vkbSwapchain.extent;
    //store swapchain and its related images
	mSwapchain = vkbSwapchain.swapchain;
	mSwapchainImages = vkbSwapchain.get_images().value();
	mSwapchainImageViews = vkbSwapchain.get_image_views().value();
}


void idk::gfx::RenderEngine::destroy_swapchain()
{
	vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);
	for (size_t i=0; i<mSwapchainImageViews.size(); i++)
    {
		vkDestroyImageView(mDevice, mSwapchainImageViews[i], nullptr);
	}
}

*/