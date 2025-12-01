#pragma once

#include <string>

#include "imgui_impl_glfw.h"

class Kernel
{
public:
    Kernel();
    ~Kernel();

    std::string init();
    void shutdown();

    int isOpen();
    void newFrame();
    void render();
    void close();

    GLFWwindow* window = nullptr;
};
