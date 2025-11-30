#include <vector>
#include <string>
#include <algorithm>
#include <cstdarg>
#include <stdio.h>

#include "Console.hpp"

Console::Console()
{
    memset(inputBuf, 0, sizeof(inputBuf));
    historyPos = -1;
    autoScroll = true;
    commands.push_back("help");
    commands.push_back("clear");
    commands.push_back("echo");
    addLog("Bem-vindo ao Terminal! Digite 'help'.");
}

Console::~Console()
{
    clearLog();
}

void Console::addLog(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, args);
    buf[sizeof(buf)-1] = 0;
    va_end(args);
    items.push_back(std::string(buf));
}

void Console::clearLog()
{
    items.clear();
}

void Console::execCommand(const char* command)
{
    addLog("# %s", command);
    if (!command || !*command) return;
    std::string cmd(command);
    std::string cmd_up = cmd;
    std::transform(cmd_up.begin(), cmd_up.end(), cmd_up.begin(), ::toupper);

    if (cmd_up == "CLEAR") {
        clearLog();
    } else if (cmd_up == "HELP") {
        addLog("Comandos disponíveis:");
        for (const auto& c : commands) addLog("- %s", c.c_str());
    } else if (cmd_up.rfind("ECHO ", 0) == 0) {
        addLog("%s", command + 5);
    } else {
        addLog("Comando desconhecido: '%s'", command);
    }

    historyPos = -1;
}

int Console::textEditCallback(ImGuiInputTextCallbackData* data)
{
    Console* console = (Console*)data->UserData;
    switch (data->EventFlag) {
        case ImGuiInputTextFlags_CallbackCompletion: {
            const char* buf = data->Buf;
            int buf_len = (int)strlen(buf);
            std::vector<const char*> candidates;
            for (size_t i = 0; i < console->commands.size(); i++) {
                const char* c = console->commands[i].c_str();
                bool match = true;
                for (int j = 0; j < buf_len; ++j) {
                    if (toupper(buf[j]) != toupper(c[j])) { match = false; break; }
                }
                if (match) candidates.push_back(c);
            }
            if (candidates.empty()) {
                console->addLog("Nenhuma correspondência para \"%s\".", buf);
            } else if (candidates.size() == 1) {
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, candidates[0]);
                data->InsertChars((int)strlen(candidates[0]), " ");
            } else {
                console->addLog("Possíveis correspondências:");
                for (const auto& c : candidates) console->addLog("- %s", c);
            }
            break;
        }
        case ImGuiInputTextFlags_CallbackHistory: {
            const int prev_history_pos = console->historyPos;
            if (data->EventKey == ImGuiKey_UpArrow) {
                if (console->historyPos == -1)
                    console->historyPos = (int)console->items.size() - 1;
                else if (console->historyPos > 0)
                    console->historyPos--;
            } else if (data->EventKey == ImGuiKey_DownArrow) {
                if (console->historyPos != -1)
                    if (++console->historyPos >= (int)console->items.size())
                        console->historyPos = -1;
            }
            if (prev_history_pos != console->historyPos) {
                data->DeleteChars(0, data->BufTextLen);
                if (console->historyPos != -1)
                    data->InsertChars(0, console->items[console->historyPos].c_str());
            }
        }
    }
    return 0;
}

void Console::draw(const char* title, bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Limpar")) clearLog();
    ImGui::SameLine();
    if (ImGui::Button("Copiar")) {
        std::string all;
        for (const auto& s : items) all += s; all += "\n";
        ImGui::SetClipboardText(all.c_str());
    }
    ImGui::SameLine();
    filter.Draw("Filtro", 180);
    ImGui::Separator();

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);

    if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::Selectable("Limpar")) clearLog();
        ImGui::EndPopup();
    }

    for (int i = 0; i < (int)items.size(); i++) {
        const char* item = items[i].c_str();
        if (!filter.PassFilter(item)) continue;
        ImGui::TextUnformatted(item);
    }

    if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::Separator();

    bool reclaim_focus = false;
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue
                                | ImGuiInputTextFlags_CallbackHistory
                                | ImGuiInputTextFlags_CallbackCompletion;

    if (ImGui::InputText("Entrada", inputBuf, IM_ARRAYSIZE(inputBuf), flags, &Console::textEditCallback, (void*)this)) {
        char* s = inputBuf;
        if (s[0]) execCommand(s);
        strcpy(s, "");
        reclaim_focus = true;
    }

    ImGui::SetItemDefaultFocus();
    if (reclaim_focus) ImGui::SetKeyboardFocusHere(-1);

    ImGui::End();
}
