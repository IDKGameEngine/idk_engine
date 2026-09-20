#pragma once

#include "IPlatformFeature.hpp"


namespace idk
{
    class FilesystemManager: public idk::IPlatformFeature
    {
    private:
        
    public:
        FilesystemManager();
        virtual void onUpdate(idk::PlatformContext&) final {  };
        virtual size_t getFileSize(const char *filepath) final;
        virtual size_t loadFile(const char *filepath, void *buf, size_t bufsz) final;
    };
}
