#include "idk/editor/Editor.hpp"
// #include "idk/engine/Engine.hpp"

// #include "idk/platform/PlatformContext.hpp"
// #include "idk/platform/EventManager.hpp"
// #include "idk/platform/VideoManager.hpp"

// #include "idk/gfx/GfxService.hpp"

// #include <imgui.h>
// #include <imgui_impl_sdl3.h>
// #include <imgui_impl_vulkan.h>

// #include <SDL3/SDL.h>


// static void check_vk_result(VkResult err)
// {
//     if (err == VK_SUCCESS)
//         return;
//     fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
//     if (err < 0)
//         abort();
// }


idk::editor::EditorApplication::EditorApplication()
{

}


// static void EditorEventCallback(const void *event)
// {
//     (void)event;
//     VLOG_INFO("[EditorEventCallback] Hello!");
//     // ImGui_ImplSDL3_ProcessEvent((const SDL_Event*)event);
// }


void idk::editor::EditorApplication::onInit(idk::Engine &E)
{
    (void)E;
    // auto *plat  = E.getService<idk::PlatformContext>();
    // auto *video = plat->getService<idk::VideoManager>();

    // auto *gfx = E.getService<idk::gfx::GfxService>();
    // auto &ren = gfx->getRenderEngine();

    // // 1. Create descriptor pool for IMGUI
    // // ---------------------------------------------------------------------------------------------
	// //  the size of the pool is very oversize, but it's copied from imgui demo itself.
	// static VkDescriptorPoolSize pool_sizes[] = { { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
	// 	{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
	// 	{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
	// 	{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
	// 	{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
	// 	{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
	// 	{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
	// 	{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
	// 	{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
	// 	{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
	// 	{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 } };

	// static VkDescriptorPoolCreateInfo pool_info = {};
	// pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	// pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	// pool_info.maxSets = 1000;
	// pool_info.poolSizeCount = (uint32_t)std::size(pool_sizes);
	// pool_info.pPoolSizes = pool_sizes;

	// static VkDescriptorPool imguiPool;
	// VK_CHECK( vkCreateDescriptorPool(ren.mDevice, &pool_info, nullptr, &imguiPool) );
    // // ---------------------------------------------------------------------------------------------


    // // 2. Setup Dear ImGui context
    // // ---------------------------------------------------------------------------------------------
    // IMGUI_CHECKVERSION();

    // ImGui::CreateContext();
    // ImGui_ImplSDL3_InitForVulkan((SDL_Window*)(video->getWindowHandle()));

    // static ImGui_ImplVulkan_InitInfo init_info = {  };
    // init_info.ApiVersion = VK_API_VERSION_1_3;
    // init_info.Instance = ren.mInstance;
    // init_info.PhysicalDevice = ren.mDevices[ren.mDeviceIndex];
    // init_info.Device = ren.mDevice;
    // init_info.QueueFamily = ren.mQueueFamily;
    // init_info.Queue = ren.mGraphicsQueue;
    // init_info.DescriptorPool = imguiPool;
    // init_info.DescriptorPoolSize = 0;
    // init_info.MinImageCount = ren.NUM_FRAMES_IN_FLIGHT;
    // init_info.ImageCount = ren.NUM_FRAMES_IN_FLIGHT;
    // init_info.UseDynamicRendering = true;
    // // init_info.Allocator = g_Allocator;
    // init_info.PipelineInfoMain.RenderPass = VK_NULL_HANDLE;
    // init_info.PipelineInfoMain.Subpass = 0;
    // init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    // init_info.CheckVkResultFn = check_vk_result;

    // ImGui_ImplVulkan_Init(&init_info);

    // plat->getService<idk::EventManager>()->addEventCallback(EditorEventCallback);
}


void idk::editor::EditorApplication::onUpdate(idk::Engine &E)
{
    (void)E;
    // auto *gfx = E.getService<idk::gfx::GfxService>();
    // auto &ren = gfx->getRenderEngine();

    // // Start the Dear ImGui frame
    // ImGui_ImplVulkan_NewFrame();
    // ImGui_ImplSDL3_NewFrame();
    // ImGui::NewFrame();

    // // --- Create your UI here ---
    // ImGui::Begin("Hello, Vulkan!");
    // // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    // ImGui::End();

    // // Render ImGui data
    // ImGui::Render();
    // ImDrawData* draw_data = ImGui::GetDrawData();

    // // --- Vulkan Command Buffer Recording ---
    // // Frame submission code (vkBeginCommandBuffer, vkCmdBeginRenderPass, etc.)
    
    // // Record ImGui primitives into your command buffer
    // ImGui_ImplVulkan_RenderDrawData(draw_data, ren.mFrames[ren.mFrameIdx].commandBuffer);

    // // End RenderPass, Submit Queue, and Present Frame
}


void idk::editor::EditorApplication::onShutdown(idk::Engine &E)
{
    (void)E;
}

