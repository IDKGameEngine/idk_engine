#include "VulkanContext.hpp"
#include "libidk/Assert.hpp"
#include "libidk/log.hpp"
#include "VkBootstrap.h"

#include <SDL3/SDL_vulkan.h>
#include <vector>

static constexpr bool bUseValidationLayers = false;


idk::gfx::VulkanContext::VulkanContext(void *win)
:   mWinHandle(win),
    mInstance(VK_NULL_HANDLE),
    mPhysicalDevice(VK_NULL_HANDLE),
    mDevice(VK_NULL_HANDLE),
    mSurface(VK_NULL_HANDLE)
{
	init_vulkan();
	init_swapchain();
	init_commands();
	init_sync_structures();

	// //make the vulkan instance, with basic debug features
	// auto inst_ret = builder.set_app_name("Example Vulkan Application")
	// 	.request_validation_layers(bUseValidationLayers)
	// 	.use_default_debug_messenger()
	// 	.require_api_version(1, 3, 0)
	// 	.build();

	// vkb::Instance vkb_inst = inst_ret.value();

	// //grab the instance 
	// _instance = vkb_inst.instance;
	// _debug_messenger = vkb_inst.debug_messenger;


    // // Instance Creation
    // // -------------------------------------------------------------------------------------------------------------------------
    // VK_CHECK( volkInitialize() );

    // VkApplicationInfo appInfo = {
    //     .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    //     .pApplicationName = "How to Vulkan",
    //     .apiVersion = VK_API_VERSION_1_3
    // };

    // uint32_t instanceExtCount = 0;
    // const char *const *instanceExtensions = SDL_Vulkan_GetInstanceExtensions(&instanceExtCount);
    // VkInstanceCreateInfo instanceCI = {
    //     .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    //     .pApplicationInfo = &appInfo,
    //     .enabledExtensionCount = instanceExtCount,
    //     .ppEnabledExtensionNames = instanceExtensions,
    // };

    // VK_CHECK( vkCreateInstance(&instanceCI, nullptr, &mInstance) );
    // volkLoadInstance(mInstance);
    // // -------------------------------------------------------------------------------------------------------------------------


    // // Device Enumeration
    // // -------------------------------------------------------------------------------------------------------------------------
    // uint32_t deviceCount = 0;
    // uint32_t deviceIndex = 0;
    // VK_CHECK( vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr) );

    // mPhysicalDevices.resize(deviceCount);
    // VK_CHECK( vkEnumeratePhysicalDevices(mInstance, &deviceCount, &mPhysicalDevices[0]) );

    // VkPhysicalDeviceProperties2 props = {
    //     .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2
    // };
    // vkGetPhysicalDeviceProperties2(mPhysicalDevices[deviceIndex], &props);

    // VLOG_INFO("[VulkanContext::VulkanContext] deviceCount: {}", deviceCount);
    // VLOG_INFO("[VulkanContext::VulkanContext] deviceName:  {}", props.properties.deviceName);
    // // -------------------------------------------------------------------------------------------------------------------------


    // // Queue Creation
    // // -------------------------------------------------------------------------------------------------------------------------
    // uint32_t queueFamilyCount = 0;

    // vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevices[deviceIndex], &queueFamilyCount, nullptr);
    // VLOG_INFO("queueFamilyCount: {}", queueFamilyCount);
    
    // std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    // vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevices[deviceIndex], &queueFamilyCount, queueFamilies.data());
    
    // for (size_t i = 0; i < queueFamilies.size(); i++)
    // {
    //     if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
    //     {
    //         VLOG_INFO("queueFamily {} has VK_QUEUE_GRAPHICS_BIT", i);
    //         break;
    //     }
    // }
    // // -------------------------------------------------------------------------------------------------------------------------
}


idk::gfx::VulkanContext::~VulkanContext()
{
    if (mInstance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(mInstance, nullptr);
        mInstance = VK_NULL_HANDLE;

        destroy_swapchain();
    
        vkDestroyDevice(mDevice, nullptr);
        vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
        vkb::destroy_debug_utils_messenger(mInstance, mDebugMessenger);
    }
}


void idk::gfx::VulkanContext::init_vulkan()
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

	//vulkan 1.2 features
	VkPhysicalDeviceVulkan12Features features12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
	features12.bufferDeviceAddress = true;
	features12.descriptorIndexing = true;

	// Use vkbootstrap to select a gpu. 
	// We want a gpu that can write to the SDL surface and supports vulkan 1.3 with the correct features
	vkb::PhysicalDeviceSelector selector{ vkb_inst };
	vkb::PhysicalDevice physicalDevice = selector
		.set_minimum_version(1, 3)
		.set_required_features_13(features)
		.set_required_features_12(features12)
		.set_surface(mSurface)
		.select()
		.value();


	// Create the final vulkan device
	vkb::DeviceBuilder deviceBuilder{ physicalDevice };

	vkb::Device vkbDevice = deviceBuilder.build().value();

	// Get the VkDevice handle used in the rest of a vulkan application
	mDevice = vkbDevice.device;
	mPhysicalDevice = physicalDevice.physical_device;
}


void idk::gfx::VulkanContext::init_swapchain()
{
    int w=0, h=0;
    SDL_GetWindowSize((SDL_Window*)mWinHandle, &w, &h);
    create_swapchain(w, h);
}


void idk::gfx::VulkanContext::init_commands()
{
    //nothing yet
}


void idk::gfx::VulkanContext::init_sync_structures()
{
    //nothing yet
}


void idk::gfx::VulkanContext::create_swapchain(uint32_t w, uint32_t h)
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


void idk::gfx::VulkanContext::destroy_swapchain()
{
	vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);
	for (size_t i=0; i<mSwapchainImageViews.size(); i++)
    {
		vkDestroyImageView(mDevice, mSwapchainImageViews[i], nullptr);
	}
}


