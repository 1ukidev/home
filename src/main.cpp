#include <cstdlib>
#include <print>

#include "imgui.h"
#include "home/Constants.hpp"
#include "home/Kernel.hpp"

int main(int, char**)
{
    std::println("Home - {}", Constants::VERSION);
    std::println("Iniciando...");

    Kernel kernel;

    auto error = kernel.init();
    if (error) {
        std::println(stderr, "Erro ao inicializar o kernel: {}", *error);
        return EXIT_FAILURE;
    }

    while (kernel.loop()) {
        kernel.newFrame();

        static bool show_window = false;

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Ferramentas")) {
                if (ImGui::MenuItem("Teste"))
                    show_window = true;
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (show_window) {
            if (ImGui::Begin("Início", &show_window)) {
                ImGui::Text("Olá, Mundo!");
                if (ImGui::Button("Fechar"))
                    show_window = false;
            }
            ImGui::End();
        }

        kernel.render();
    }

    kernel.shutdown();

    return EXIT_SUCCESS;
}
