#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <cstddef>

class Tetromino
{
    public:
        // 特殊函数
        Tetromino();
    public:
        // 普通函数
        void rotate();
        std::vector<std::vector<int>> getRotatedShape() const;
    public:
        // 变量
        int m_x;
        int m_y;
        int m_shapeIndex;
        std::vector<std::vector<int>> m_shape;
        sf::Color m_color;
    public:
        // 静态变量
        static const std::vector<sf::Color> COLORS;
    private:
        // 静态变量
        static const std::vector<std::vector<std::vector<int>>> SHAPES;
};

inline Tetromino::Tetromino()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> shapeDist(0, static_cast<int>(SHAPES.size()) - 1);
    m_shapeIndex = shapeDist(gen);
    m_shape = SHAPES[static_cast<std::size_t>(m_shapeIndex)];
    m_color = COLORS[static_cast<std::size_t>(m_shapeIndex)];

    // 初始化位置在顶部中间
    m_x = 5 - static_cast<int>(m_shape[0].size()) / 2;
    m_y = 0;
}

inline void Tetromino::rotate()
{
    std::vector<std::vector<int>> rotated(m_shape[0].size(), std::vector<int>(m_shape.size()));
    for (std::size_t i = 0; i < m_shape.size(); i++)
    {
        for (std::size_t j = 0; j < m_shape[i].size(); j++)
        {
            rotated[j][m_shape.size() - 1 - i] = m_shape[i][j];
        }
    }
    m_shape = rotated;
}

inline std::vector<std::vector<int>> Tetromino::getRotatedShape() const
{
    std::vector<std::vector<int>> rotated(m_shape[0].size(), std::vector<int>(m_shape.size()));
    for (std::size_t i = 0; i < m_shape.size(); i++)
    {
        for (std::size_t j = 0; j < m_shape[i].size(); j++)
        {
            rotated[j][m_shape.size() - 1 - i] = m_shape[i][j];
        }
    }
    return rotated;
}