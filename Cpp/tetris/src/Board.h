#pragma once

#include "Tetromino.h"
#include <vector>
#include <cstddef>

class Board
{
    public:
        // 特殊函数
        Board();
    public:
        // 静态变量
        static const int WIDTH;
        static const int HEIGHT;
        static const int CELL_SIZE;
    public:
        // 普通函数
        void reset();
        bool checkCollision(const Tetromino& piece, int dx = 0, int dy = 0) const;
        void lockPiece(const Tetromino& piece);
        int clearLines();
        bool isGameOver() const;
        const std::vector<std::vector<int>>& getGrid() const;
    private:
        std::vector<std::vector<int>> m_grid;
        bool gameOver;
};

inline Board::Board() : m_grid(static_cast<std::size_t>(HEIGHT), std::vector<int>(static_cast<std::size_t>(HEIGHT), 0)), gameOver(false)
{

}

inline void Board::reset()
{
    m_grid = std::vector<std::vector<int>>(static_cast<std::size_t>(HEIGHT), std::vector<int>(static_cast<std::size_t>(WIDTH), 0));
    gameOver = false;
}

inline bool Board::checkCollision(const Tetromino& piece, int dx, int dy) const
{
    bool ret = false;
    for (std::size_t i = 0; i < piece.m_shape.size(); i++)
    {
        for (std::size_t j = 0; j < piece.m_shape[i].size(); j++)
        {
            if (piece.m_shape[i][j])
            {
                int boardX = piece.m_x + static_cast<int>(j) + dx;
                int boardY = piece.m_y + static_cast<int>(i) + dy;
                if (boardX < 0 || boardX >= WIDTH || boardY >= HEIGHT || (boardY >= 0 && m_grid[static_cast<std::size_t>(boardY)][static_cast<std::size_t>(boardX)]))
                {
                    ret = true;
                }
            }
        }
    }
    return ret;
}

inline void Board::lockPiece(const Tetromino& piece)
{
    for (std::size_t i = 0; i < piece.m_shape.size(); i++)
    {
        for (std::size_t j = 0; j < piece.m_shape[i].size(); j++)
        {
            if (piece.m_shape[i][j])
            {
                int boardX = piece.m_x + static_cast<int>(j);
                int boardY = piece.m_y + static_cast<int>(i);
                if (boardY >= 0)
                {
                    m_grid[static_cast<std::size_t>(boardY)][static_cast<std::size_t>(boardX)] = piece.m_shapeIndex + 1;
                }
            }
        }
    }
    gameOver = false;
    for (int i = 0; i < WIDTH; i++)
    {
        if (m_grid[0][static_cast<std::size_t>(i)] != 0)
        {
            gameOver = true;
            break;
        }
    }
}

inline int Board::clearLines()
{
    int linesCleared = 0;
    for (int i = HEIGHT - 1; i >= 0; i--)
    {
        bool lineComplete = true;
        for (int j = 0; j < WIDTH; j++)
        {
            if (m_grid[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)] == 0)
            {
                lineComplete = false;
                break;
            }
        }
        if (lineComplete)
        {
            linesCleared++;
            for (int k = i; k > 0; k--)
            {
                m_grid[static_cast<std::size_t>(k)] = m_grid[static_cast<std::size_t>(k - 1)];
            }
            m_grid[0] = std::vector<int>(static_cast<std::size_t>(WIDTH), 0);
            i++;
        }
    }
    return linesCleared;
}

inline bool Board::isGameOver() const
{
    return gameOver;
}

inline const std::vector<std::vector<int>>& Board::getGrid() const
{
    return m_grid;
}