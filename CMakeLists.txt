cmake_minimum_required(VERSION 3.20)
project(home LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

add_compile_definitions(VERSION_NUMBER="${PROJECT_VERSION}")

if(NOT EMSCRIPTEN)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
endif()

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/external/imgui")

add_library(imgui_static STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_static PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

file(GLOB_RECURSE PROJECT_SOURCES
    "${SRC_DIR}/*.cpp"
)

add_executable(home
    ${PROJECT_SOURCES}
)

set(SPDLOG_DIR "${CMAKE_SOURCE_DIR}/external/spdlog")
add_subdirectory(${SPDLOG_DIR})

if(EMSCRIPTEN)
    target_link_options(home PRIVATE
        "-sUSE_GLFW=3"
        "-sUSE_WEBGL2=1"
        "-sFULL_ES3=1"
        "-sWASM=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "--shell-file" "${CMAKE_SOURCE_DIR}/shell.html"
    )

    set_target_properties(home PROPERTIES
        OUTPUT_NAME "index"
        SUFFIX ".html"
    )

    target_link_libraries(home PRIVATE
        imgui_static
        spdlog::spdlog
    )
else()
    target_link_libraries(home PRIVATE
        imgui_static
        spdlog::spdlog
        OpenGL::GL
        glfw
    )
endif()
