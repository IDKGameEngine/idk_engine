project(idkengine_gfx)

find_package(volk CONFIG REQUIRED)

add_library(
    ${PROJECT_NAME} OBJECT
)
target_compile_features(
    ${PROJECT_NAME} PUBLIC cxx_std_23
)
target_compile_options(
    ${PROJECT_NAME} PRIVATE -Wno-missing-field-initializers
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(
    ${PROJECT_NAME} PUBLIC idkengine_platform PRIVATE volk::volk 
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/vk/GraphicsDevice.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/GfxService.cpp"
)
