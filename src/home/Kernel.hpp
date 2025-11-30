#pragma once

#include <optional>
#include <string>

#include "imgui_impl_glfw.h"

class Kernel
{
public:
    Kernel();
    ~Kernel();

    std::optional<std::string> init();
    void shutdown();

    int loop();
    void newFrame();
    void render();
    void close();

    GLFWwindow* window{nullptr};
};
