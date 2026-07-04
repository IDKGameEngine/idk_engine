cmake_minimum_required(VERSION 4.2)

project(idk_engine VERSION 0.0.1)

add_library(
    ${PROJECT_NAME} STATIC
)
target_compile_features(
    ${PROJECT_NAME} PUBLIC cxx_std_23
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/h"
)
target_link_libraries(
    ${PROJECT_NAME} PUBLIC idk_core Jolt::Jolt SDL3::SDL3-shared SDL3_net::SDL3_net
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Engine.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/IoService.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/platform.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/GameServerService.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/GameClientService.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/UdpRxTxer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/World.cpp"
)

add_custom_target(
    idk_engine_data
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/${IDK_ASSETS_DIRNAME} ${IDK_OUTPUT_DIR}/${IDK_ASSETS_DIRNAME}
)
add_dependencies(${PROJECT_NAME} idk_engine_data)
