#include <string_view>
#include <GLFW/glfw3.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Constants.hpp"
#include "Kernel.hpp"
#include "Fonts.hpp"

Kernel::Kernel() = default;

Kernel::~Kernel()
{
#ifndef __EMSCRIPTEN__
    shutdown();
#endif
}

std::string_view Kernel::init()
{
    if (running)
        return "já está em execução";

    if (!glfwInit())
        return "falha ao inicializar o GLFW";

#ifdef __EMSCRIPTEN__
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

#ifdef __EMSCRIPTEN__
    double width, height;
    emscripten_get_element_css_size("#canvas", &width, &height);
#else
    constexpr double width = Constants::W_WIDTH;
    constexpr double height = Constants::W_HEIGHT;
#endif

    window = glfwCreateWindow((int)width, (int)height, Constants::TITLE.data(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return "falha ao criar a janela GLFW";
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontFromMemoryCompressedTTF(roboto_compressed_data, roboto_compressed_size, Constants::FONT_SIZE);

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    running = true;
    
    return "";
}

#ifndef __EMSCRIPTEN__
void Kernel::shutdown()
{
    if (!running) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    running = false;
}

void Kernel::close()
{
    if (!running) return;
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

bool Kernel::isOpen() const
{
    return running && !glfwWindowShouldClose(window);
}
#endif

bool Kernel::isRunning() const
{
    return running;
}

GLFWwindow* Kernel::getWindow() const
{
    return window;
}

void Kernel::newFrame()
{
    if (!running) return;

#ifndef __EMSCRIPTEN__
    glfwWaitEventsTimeout(0.034);
#endif

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Kernel::render()
{
    if (!running) return;

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    glClearColor(Constants::BG_R, Constants::BG_G, Constants::BG_B, Constants::BG_A);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}
