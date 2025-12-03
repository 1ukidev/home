#pragma once

#include <string_view>

class Constants
{
public:
    static constexpr std::string_view TITLE = "Home";
    static constexpr std::string_view VERSION = VERSION_NUMBER;

    static constexpr double W_WIDTH = 1000;
    static constexpr double W_HEIGHT = 600;
    static constexpr float FONT_SIZE = 16.0f;

    static constexpr float BG_R = 0.067f;
    static constexpr float BG_G = 0.067f;
    static constexpr float BG_B = 0.067f;
    static constexpr float BG_A = 1.0f;
};
