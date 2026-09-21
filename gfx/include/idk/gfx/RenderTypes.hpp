#pragma once

#include "Handle.hpp"
#include "libidk/gpu//gl.hpp"
#include "libidk/dsa/List.hpp"

// #include <glm


namespace idk::gfx
{
    // ---------------------------------------------------------------
    // Resource descriptors: how other systems describe what they want,
    // without knowing how it maps onto OpenGL.
    // ---------------------------------------------------------------
 
    enum class VertexAttribType : uint8_t
    {
        Float,
        Float2,
        Float3,
        Float4,
        UByte4Norm,
    };
 
    struct VertexAttribute
    {
        uint32_t         location = 0;
        VertexAttribType type     = VertexAttribType::Float3;
        uint32_t         offset   = 0;
    };
 
    struct MeshDesc
    {
        const void*     vertexData   = nullptr;
        size_t          vertexCount  = 0;
        size_t          vertexStride = 0;
 
        const uint32_t* indices      = nullptr;
        size_t          indexCount   = 0;
 
        // idk::dsa::List<VertexAttribute> attributes;
    };
 
    enum class TexFormat : uint8_t
    {
        R8,
        RGB8,
        RGBA8,
        RGBA32F,
    };
 
    struct TextureDesc
    {
        uint32_t    width      = 0;
        uint32_t    height     = 0;
        TexFormat   format     = TexFormat::RGBA8;
        const void* pixels     = nullptr;
        bool        genMipmaps = true;
    };
 
    struct ShaderDesc
    {
        std::string_view vertSrc;
        std::string_view fragSrc;
    };
 
    struct MaterialDesc
    {
        ShaderHandle shader;
        // TODO: named uniform/texture bindings once something needs them.
    };
 
    // ---------------------------------------------------------------
    // Per-frame submission
    // ---------------------------------------------------------------
 
    struct Camera
    {
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
    };
 
    struct DrawItem
    {
        MeshHandle     mesh;
        MaterialHandle material;
        glm::mat4      transform = glm::mat4(1.0f);
    };


    class RenderEngine: public idk::Service
    {
    public:
        virtual void onInit(EngineAPI&) final;
        virtual void onShutdown(EngineAPI&) final;
        virtual void onPreFrame(EngineAPI&) final;
        virtual void onMidFrame(EngineAPI&) final;
        virtual void onPostFrame(EngineAPI&) final;
        virtual void onEvent(EngineAPI&, const void*) final;


        MeshHandle     CreateMesh(const MeshDesc&);
        void           DestroyMesh(MeshHandle);
        TextureHandle  CreateTexture(const TextureDesc&);
        void           DestroyTexture(TextureHandle);
        ShaderHandle   CreateShader(const ShaderDesc&);
        MaterialHandle CreateMaterial(const MaterialDesc&);

    // private:
        void      *mWinCtx;
        void      *mGlCtx;
        uint32_t   mDummyVao;

    };
}
