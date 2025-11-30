cmake_minimum_required(VERSION 3.20)
project(home LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_compile_definitions(VERSION_NUMBER="${PROJECT_VERSION}")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(spdlog REQUIRED)

set(IMGUI_DIR "external/imgui")

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

target_include_directories(home PRIVATE
    ${SRC_DIR}
)

target_link_libraries(home PRIVATE 
    imgui_static 
    glfw 
    OpenGL::GL
    spdlog::spdlog
)
