// #include "idk/gfx/GfxService.hpp"
// #include "idk/gfx/RenderEngine.hpp"

// #include "idk/platform/VideoManager.hpp"
// #include "libidk/New.hpp"

// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>


// idk::gfx::GfxService::GfxService()
// :   mRenderEngine()
// {
//     Assimp::Importer importer;

//     uint32_t flags  = aiProcess_FlipUVs;
//              flags |= aiProcess_OptimizeMeshes;
//              flags |= aiProcess_OptimizeGraph;
//              flags |= aiProcess_CalcTangentSpace;
//              flags |= aiProcess_LimitBoneWeights;
//              flags |= aiProcess_PopulateArmatureData;
 
//     const aiScene *scene = importer.ReadFile("model/gun/gun.obj", flags);
//     VLOG_INFO("scene->mNumMeshes: {}", scene->mNumMeshes);
// }


// idk::gfx::GfxService::~GfxService()
// {

// }


// void idk::gfx::GfxService::onInit(EngineAPI&)
// {

// }


// void idk::gfx::GfxService::onPreFrame(EngineAPI&)
// {
//     mRenderEngine.update();
// }


// void idk::gfx::GfxService::onShutdown(EngineAPI&)
// {
    
// }


// void idk::gfx::GfxService::onEvent(EngineAPI&, const void*)
// {

// }

