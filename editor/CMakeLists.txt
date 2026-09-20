project(editor)

add_executable(
    ${PROJECT_NAME}
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
    PUBLIC idkengine_common idkengine_platform idkengine_gfx idkengine_engine
    PRIVATE imgui
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Editor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp"
)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION "${CMAKE_INSTALL_PREFIX}"
)
