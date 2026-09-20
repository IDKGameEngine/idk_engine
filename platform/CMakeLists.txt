project(idkengine_platform)

find_package(Steamworks CONFIG REQUIRED)

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
    ${PROJECT_NAME}
    PUBLIC SDL3 SDL3_net SDL3_mixer libidk idkengine_common
    PRIVATE Steamworks::Steamworks
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/AudioManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/EventManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/FilesystemManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/InputManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/PlatformContext.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/VideoManager.cpp"
)