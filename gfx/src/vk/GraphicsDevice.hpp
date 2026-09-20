#pragma once

#include "libidk/gpu/vk.hpp"


namespace idk::gfx
{
    class GraphicsDevice
    {
    public:
        GraphicsDevice();
        ~GraphicsDevice();

    private:
        VkInstance       mInstance;
        VkSurfaceKHR     mSurface;
        VkPhysicalDevice mPhysicalDevice;
        VkDevice         mDevice;
        VkQueue          mGraphicsQueue;
        VkQueue          mPresentQueue;
    };
}

