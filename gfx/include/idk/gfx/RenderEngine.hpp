#pragma once

#include "libidk/gpu//gl.hpp"
#include "libidk/dsa/List.hpp"
#include "idk/EngineAPI.hpp"


namespace idk::gfx
{
    class RenderEngine: public idk::Service
    {
    public:
        virtual void onInit(EngineAPI&) final;
        virtual void onShutdown(EngineAPI&) final;
        virtual void onPreFrame(EngineAPI&) final;
        virtual void onMidFrame(EngineAPI&) final;
        virtual void onPostFrame(EngineAPI&) final;
        virtual void onEvent(EngineAPI&, const void*) final;

        // MeshHandle     CreateMesh(const MeshDesc&);
        // void           DestroyMesh(MeshHandle);
        // TextureHandle  CreateTexture(const TextureDesc&);
        // void           DestroyTexture(TextureHandle);
        // ShaderHandle   CreateShader(const ShaderDesc&);
        // MaterialHandle CreateMaterial(const MaterialDesc&);

    // private:
        void      *mWinCtx;
        void      *mGlCtx;
        uint32_t   mDummyVao;

    };
}
