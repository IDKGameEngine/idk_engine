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
    ${PROJECT_NAME} PUBLIC idk_core glm::glm-header-only Jolt::Jolt
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/controller.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/engine.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/io_api.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/io_service.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/OsAdapter.cpp"
)