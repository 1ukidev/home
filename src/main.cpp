// Copyright (c) Leonardo Moreira <leo.monteiro06@live.com>. Licensed under the MIT Licence.
// See the LICENCE file in the repository root for full licence text.

#include <cstdlib>
#include <string_view>
#include <spdlog/spdlog.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "imgui.h"
#include "Constants.hpp"
#include "Kernel.hpp"
#include "Console.hpp"

static bool showMenuBar = true;
static bool showSobreMim = false;
static bool showConsole = false;

static Kernel kernel;
static Console console;

static void createMenuBar();
static void createSobreMim();
static void createConsole();

static void createMenuBar()
{
    if (showMenuBar) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::MenuItem("Sobre mim"))
                showSobreMim = true;

            if (ImGui::BeginMenu("Ferramentas")) {
                if (ImGui::MenuItem("Terminal"))
                    showConsole = true;
                ImGui::EndMenu();
            }

#ifndef __EMSCRIPTEN__
            if (ImGui::MenuItem("Encerrar"))
                kernel.close();
#endif

            ImGui::EndMainMenuBar();
        }
    }
}

static void createSobreMim()
{
    if (showSobreMim) {
        if (ImGui::Begin("Sobre mim", &showSobreMim)) {
            ImGui::Text("Chave PGP: F0258C27407E9095");
            ImGui::Text("GitHub: 1ukidev");
            ImGui::Text("LinkedIn: leonardomm");
            ImGui::Text("E-mail: leo.monteiro06@protonmail.com");
        }
        ImGui::End();
    }
}

static void createConsole()
{
    if (showConsole)
        console.draw("Terminal", &showConsole);
}

static void loop(void*)
{
    kernel.newFrame();

    createMenuBar();
    createSobreMim();
    createConsole();

    kernel.render();
}

int main(int, char**)
{
    spdlog::info("Home - v{}", Constants::VERSION);

    spdlog::info("Inicializando...");
    std::string_view error = kernel.init();
    if (!error.empty()) {
        spdlog::error("Erro ao inicializar o kernel: {}", error);
        return EXIT_FAILURE;
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(loop, nullptr, 0, EM_TRUE);
#else
    while (kernel.isOpen())
        loop(nullptr);

    spdlog::info("Encerrando...");
    kernel.shutdown();

    return EXIT_SUCCESS;
#endif
}
