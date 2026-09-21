#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class VideoManager: public idk::IPlatformFeature
    {
    private:
        const char      *mTitle;
        void            *mWin;
        int32_t          mWidth;
        int32_t          mHeight;

    public:
        VideoManager(const char *title, int w, int h);
        ~VideoManager();

        virtual void onUpdate(EngineAPI&) final;
        virtual void onShutdown(EngineAPI&) final;
        virtual void onEvent(EngineAPI&, const void*) final;

        void *getWindowHandle() { return mWin; }

        int   getWidth()  { return mWidth; };
        int   getHeight() { return mHeight; };

        void  setWindowFullscreen(bool fullscreen);
        void  setWindowVisibility(bool visible);
        void  setWindowResolution(int w, int h);
        void  setRenderResolution(int w, int h);

    };
}

