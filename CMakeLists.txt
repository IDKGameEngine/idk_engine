cmake_minimum_required(VERSION 3.31)

project(idkengine VERSION 0.0.1)

add_library(${PROJECT_NAME} STATIC)
target_compile_features(
    ${PROJECT_NAME} PUBLIC cxx_std_23
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/h"
)
target_link_libraries(
    ${PROJECT_NAME} PUBLIC libidk Jolt::Jolt
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/NetService.cpp"
)

# add_custom_target(
#     idk_engine_data
#     COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
#         ${CMAKE_CURRENT_SOURCE_DIR}/${IDK_ASSETS_DIRNAME} ${IDK_OUTPUT_DIR}/${IDK_ASSETS_DIRNAME}
# )
# add_dependencies(${PROJECT_NAME} idk_engine_data)
