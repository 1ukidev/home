#pragma once

#include "imgui_impl_glfw.h"

class Kernel
{
public:
    Kernel();
    ~Kernel();

    const char* init();
    void shutdown();
#ifndef __EMSCRIPTEN__
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
