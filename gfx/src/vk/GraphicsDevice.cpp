#include "GraphicsDevice.hpp"
#include "libidk/Assert.hpp"
#include "libidk/log.hpp"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>
#include <SDL3/SDL_vulkan.h>


idk::gfx::GraphicsDevice::GraphicsDevice()
:   mInstance(VK_NULL_HANDLE),
    mSurface(VK_NULL_HANDLE),
    mPhysicalDevice(VK_NULL_HANDLE),
    mDevice(VK_NULL_HANDLE),
    mGraphicsQueue(VK_NULL_HANDLE),
    mPresentQueue(VK_NULL_HANDLE)
{
    volkInitialize();

    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "How to Vulkan",
        .apiVersion = VK_API_VERSION_1_4
    };

    uint32_t instanceExtCount = 0;
    const char *const *instanceExtensions = SDL_Vulkan_GetInstanceExtensions(&instanceExtCount);
    VkInstanceCreateInfo instanceCI = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = instanceExtCount,
        .ppEnabledExtensionNames = instanceExtensions,
    };

    IDK_ASSERT(
        VK_SUCCESS == vkCreateInstance(&instanceCI, nullptr, &mInstance),
        "[GraphicsDevice::GraphicsDevice] vkCreateInstance failure"
    );

    volkLoadInstance(mInstance);


    uint32_t deviceCount = 0;
    VkPhysicalDevice devices[8];
    IDK_ASSERT(
        VK_SUCCESS == vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr),
        "[GraphicsDevice::GraphicsDevice] vkEnumeratePhysicalDevices failure"
    );

    VLOG_INFO("[GraphicsDevice::GraphicsDevice] mDeviceCount=={}", deviceCount);

    IDK_ASSERT(
        VK_SUCCESS == vkEnumeratePhysicalDevices(mInstance, &deviceCount, &devices[0]),
        "[GraphicsDevice::GraphicsDevice] vkEnumeratePhysicalDevices failure"
    );

    VkPhysicalDeviceProperties2 props = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2
    };
    vkGetPhysicalDeviceProperties2(devices[0], &props);
    VLOG_INFO("[GraphicsDevice::GraphicsDevice] props.properties.deviceName=={}", props.properties.deviceName);
    
}



idk::gfx::GraphicsDevice::~GraphicsDevice()
{
    if (mInstance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(mInstance, nullptr);
        mInstance = VK_NULL_HANDLE;
    }
}

