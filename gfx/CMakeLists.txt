project(idkengine_gfx)

find_package(volk CONFIG REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)

add_library(
    ${PROJECT_NAME} OBJECT
)
target_compile_features(
    ${PROJECT_NAME} PUBLIC cxx_std_23
)
target_compile_options(
    ${PROJECT_NAME} PRIVATE -Wno-missing-field-initializers -Wno-unused-parameter
)
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/vk-bootstrap"
)
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC idkengine_common idkengine_platform
    PRIVATE volk::volk GPUOpen::VulkanMemoryAllocator
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/vk/vk_mem_alloc.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/GfxService.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/RenderEngine.cpp"
)
