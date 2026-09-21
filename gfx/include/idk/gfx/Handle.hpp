
#pragma once
 
#include <cstdint>
 
namespace idk::gfx
{
    // Opaque, type-safe reference to a GPU resource. Never contains a raw
    // GL id -- RenderManager is the only thing that knows how to turn one
    // of these into an actual OpenGL object, via its internal resource
    // pools (see detail/Pool.hpp). Everything outside idk/gfx/*.cpp should
    // only ever see Handle<T>, never a GLuint.
    template <typename Tag>
    struct Handle
    {
        uint32_t id  = 0;   // slot index into the owning Pool
        uint32_t gen = 0;   // generation, guards against stale handles
 
        bool isValid() const { return gen != 0; }
 
        friend bool operator==(const Handle&, const Handle&) = default;
    };
 
    struct MeshTag         {};
    struct TextureTag      {};
    struct ShaderTag       {};
    struct MaterialTag     {};
    struct RenderTargetTag {};
 
    using MeshHandle         = Handle<MeshTag>;
    using TextureHandle      = Handle<TextureTag>;
    using ShaderHandle       = Handle<ShaderTag>;
    using MaterialHandle     = Handle<MaterialTag>;
    using RenderTargetHandle = Handle<RenderTargetTag>;
}
