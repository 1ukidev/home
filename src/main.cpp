// Copyright (c) Leonardo Moreira <leo.monteiro06@live.com>. Licensed under the MIT Licence.
// See the LICENCE file in the repository root for full licence text.

#include <cstdlib>
#include <spdlog/spdlog.h>

#include "imgui.h"
#include "home/Constants.hpp"
#include "home/Kernel.hpp"

static bool showMenuBar = true;
static bool showTeste = false;

static void createMenuBar();
static void createTeste();

static void createMenuBar()
{
    if (showMenuBar) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Ferramentas")) {
                if (ImGui::MenuItem("Teste"))
                    showTeste = true;
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
}

static void createTeste()
{
    if (showTeste) {
        if (ImGui::Begin("Início", &showTeste)) {
            ImGui::Text("Olá, Mundo!");
            if (ImGui::Button("Fechar")) {
                spdlog::info("Fechando teste...");
                showTeste = false;
            }
        }
        ImGui::End();
    }
}

int main(int, char**)
{
    spdlog::info("Home - v{}", Constants::VERSION);

    Kernel kernel;

    auto error = kernel.init();
    if (error) {
        spdlog::error("Erro ao inicializar o kernel: {}", *error);
        return EXIT_FAILURE;
    }

    while (kernel.loop()) {
        kernel.newFrame();

        createMenuBar();
        createTeste();

        kernel.render();
    }

    kernel.shutdown();

    return EXIT_SUCCESS;
}
