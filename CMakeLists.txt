cmake_minimum_required(VERSION 3.31)

project(idkengine VERSION 0.0.1)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/engine")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/gfx")

add_library(
    ${PROJECT_NAME} STATIC
)
target_include_directories(
    idkengine_gfx PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/engine/include"
)
target_link_libraries(
    ${PROJECT_NAME} PUBLIC SDL3 SDL3_net SDL3_mixer idkengine_gfx idkengine_engine
)
