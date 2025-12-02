#pragma once

#include <string_view>

#include "imgui_impl_glfw.h"

class Kernel
{
public:
    Kernel();
    ~Kernel();

    std::string_view init();
#ifndef __EMSCRIPTEN__
    void shutdown();
    void close();
    bool isOpen() const;
#endif
    bool isRunning() const;
    GLFWwindow* getWindow() const;

    void newFrame();
    void render();

private:
    bool running = false;
    GLFWwindow* window = nullptr;
};
