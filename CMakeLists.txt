cmake_minimum_required(VERSION 3.31)

project(idkengine VERSION 0.0.1)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/engine")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/gfx")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/platform")

add_library(
    ${PROJECT_NAME} STATIC
)
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC idkengine_platform idkengine_gfx idkengine_engine
)
