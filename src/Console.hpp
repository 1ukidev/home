#pragma once

#include <vector>
#include <string>

#include "imgui.h"

class Console
{
public:
    Console();
    ~Console();

    void addLog(const char* fmt, ...) IM_FMTARGS(2);
    void clearLog();
    void execCommand(const char* command);

    void help();

    static int textEditCallback(ImGuiInputTextCallbackData* data);
    void draw(const char* title, bool* open = nullptr);

    char inputBuf[256];
    std::vector<std::string> items;
    std::vector<std::string> commands;
    std::vector<int> history;
    int historyPos;
    bool autoScroll;
    ImGuiTextFilter filter;
};
