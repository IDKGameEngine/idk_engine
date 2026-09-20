#pragma once

#include "IPlatformFeature.hpp"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <vector>


namespace idk
{
    class VideoManager: public idk::IPlatformFeature
    {
    private:
        const char      *mTitle;
        void            *mWin;
        VkInstance       mInstance;
        // VkSurfaceKHR     mSurface;
        // VkPhysicalDevice mPhysicalDevice;
        // VkDevice         mDevice;
        // VkQueue          mGraphicsQueue;
        int32_t          mWidth;
        int32_t          mHeight;

    public:
        VideoManager(const char *title, int w, int h);
        ~VideoManager();

        virtual void onUpdate(idk::PlatformContext&) final;

        int   getWidth()  { return mWidth; };
        int   getHeight() { return mHeight; };

        void  setWindowFullscreen(bool fullscreen);
        void  setWindowVisibility(bool visible);
        void  setWindowResolution(int w, int h);
        void  setRenderResolution(int w, int h);

    };
}

