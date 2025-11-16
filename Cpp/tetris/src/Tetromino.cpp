#include "Tetromino.h"

// 方块形状定义
const std::vector<std::vector<std::vector<int>>> Tetromino::SHAPES = 
{
    // I
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // J
    {
        {1, 0, 0},
        {1, 1, 1},
        {0, 0, 0}
    },
    // L
    {
        {0, 0, 1},
        {1, 1, 1},
        {0, 0, 0}
    },
    // O
    {
        {1, 1},
        {1, 1}
    },
    // S
    {
        {0, 1, 1},
        {1, 1, 0},
        {0, 0, 0}
    },
    // T
    {
        {0, 1, 0},
        {1, 1, 1},
        {0, 0, 0}
    },
    // Z
    {
        {1, 1, 0},
        {0, 1, 1},
        {0, 0, 0}
    }
};

// 方块颜色
const std::vector<sf::Color> Tetromino::COLORS =
{
    sf::Color::Cyan,  // I
    sf::Color::Blue,  // J
    sf::Color(255, 165, 0),  // Orange(L)
    sf::Color::Yellow,  // O
    sf::Color::Green,  // S
    sf::Color::Magenta,  // T
    sf::Color::Red  // Z
};