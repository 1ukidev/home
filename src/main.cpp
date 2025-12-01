// Copyright (c) Leonardo Moreira <leo.monteiro06@live.com>. Licensed under the MIT Licence.
// See the LICENCE file in the repository root for full licence text.

#include <cstdlib>
#include <string>
#include <print>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "imgui.h"
#include "home/Constants.hpp"
#include "home/Kernel.hpp"
#include "home/Console.hpp"

static bool showMenuBar = true;
static bool showTeste = false;
static bool showConsole = false;

static Kernel kernel;
static Console console;

static void createMenuBar();
static void createTeste();
static void createConsole();

static void createMenuBar()
{
    if (showMenuBar) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Ferramentas")) {
                if (ImGui::MenuItem("Teste"))
                    showTeste = true;
                if (ImGui::MenuItem("Terminal"))
                    showConsole = true;
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
}

static void createTeste()
{
    if (showTeste) {
        if (ImGui::Begin("Teste", &showTeste)) {
            ImGui::Text("Olá, Mundo!");
            if (ImGui::Button("Fechar")) {
                std::println("Fechando teste...");
                showTeste = false;
            }
        }
        ImGui::End();
    }
}

static void createConsole()
{
    if (showConsole)
        console.draw("Terminal", &showConsole);
}

static void loop(void* arg)
{
    kernel.newFrame();

    createMenuBar();
    createTeste();
    createConsole();

    kernel.render();
}

int main(int, char**)
{
    std::println("Home - v{}", Constants::VERSION);

    std::string error = kernel.init();
    if (!error.empty()) {
        std::println(stderr, "Erro ao inicializar o kernel: {}", error);
        return EXIT_FAILURE;
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(loop, NULL, 0, true);
#else
    while (kernel.isOpen()) {
        loop(NULL);
    }
#endif

    kernel.shutdown();

    return EXIT_SUCCESS;
}
