#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "Board.h"
#include "Tetromino.h"

#ifdef _WIN32

#include <windows.h>

#endif

class Game
{
    public:
        // 特殊函数
        Game();
    public:
        // 普通函数
        void handleInput(sf::Keyboard::Key key);
        void update();
        void draw(sf::RenderWindow& window);
        void reset();
        bool isGameOver() const;
        void saveHighScore();
    private:
        // 函数
        void spawnPiece();
        bool movePiece(int dx, int dy);
        void rotatePiece();
        void hardDrop();
        void drawBoard(sf::RenderWindow& window);
        void drawCurrentPiece(sf::RenderWindow& window);
        void drawNextPiece(sf::RenderWindow& window);
        void drawUI(sf::RenderWindow& window);
        void drawGameOver(sf::RenderWindow& window);
        void loadHighScore();
        void checkHighScore();
        void toggleMusic();
    private:
        // 变量
        Board m_board;
        Tetromino m_currentPiece;
        Tetromino m_nextPiece;
        sf::Clock m_clock;
        float m_fallSpeed;
        int m_score;
        int m_linesCleared;
        sf::Font m_font;
        int m_level;
        bool m_fontLoaded;
        sf::Music m_backgroundMusic;
        bool m_musicLoaded;
        int m_highScore;
        bool m_newHighScore;
};

inline Game::Game() : m_fallSpeed(0.5), m_score(0), m_linesCleared(0), m_level(0), m_fontLoaded(false), m_musicLoaded(false), m_highScore(0), m_newHighScore(false)
{
    // 尝试加载字体
    m_fontLoaded = m_font.openFromFile("assets/ITCKRIST.TTF");

    // 如果当前目录字体加载失败则尝试加载系统字体
    if (!m_fontLoaded)
    {
        #ifdef _WIN32

        m_fontLoaded = m_font.openFromFile("C:/Windows/Fonts/ITCKRIST.TTF");
        
        #endif
    }
    if (!m_fontLoaded)
    {
        std::cerr << "Failed to load font, text will not be displayed\n";
    }

    // 加载背景音乐
    if (m_backgroundMusic.openFromFile("assets/music/background_music.flac"))
    {
        m_musicLoaded = true;
        m_backgroundMusic.setLooping(true);  // 循环播放
        m_backgroundMusic.setVolume(100);  // 音量
        m_backgroundMusic.play();  // 播放
    }
    loadHighScore();  // 游戏启动时读取最高分
    spawnPiece();
}

inline void Game::spawnPiece()
{
    m_currentPiece = m_nextPiece;
    m_nextPiece = Tetromino();
    if (m_board.checkCollision(m_currentPiece))
    {
        m_board.lockPiece(m_currentPiece);
    }
}

inline bool Game::movePiece(int dx, int dy)
{
    bool ret = false;
    if (!isGameOver() && !m_board.checkCollision(m_currentPiece, dx, dy))
    {
        m_currentPiece.m_x += dx;
        m_currentPiece.m_y += dy;
        ret = true;
    }
    return ret;
}

inline void Game::rotatePiece()
{
    if (isGameOver())
    {
        return;
    }
    auto rotateShape = m_currentPiece.getRotatedShape();
    Tetromino temp = m_currentPiece;
    temp.m_shape = rotateShape;
    if (!m_board.checkCollision(temp))
    {
        m_currentPiece.rotate();
    }
}

inline void Game::hardDrop()
{
    while (movePiece(0, 1))
    {

    }
    m_board.lockPiece(m_currentPiece);
    int lines = m_board.clearLines();
    int blocksCleared = lines * Board::WIDTH;  // 每行有WIDTH个方块
    m_linesCleared += lines;  // 累计消除行数
    m_score += blocksCleared;
    int newLevel = m_linesCleared / 5;  // 每5行升1级
    // m_score += m_board.clearLines() * 100;
    spawnPiece();

    // 加快下落速度
    // if (m_linesCleared > 0 && m_linesCleared % 10 == 0)
    // {
    //     m_fallSpeed = std::max(0.1f, m_fallSpeed - 0.05f);
    // }

    // if (m_linesCleared > 0 && m_linesCleared % 10 == 0)
    // {
    //     m_fallSpeed = std::max(0.05f, m_fallSpeed * 0.8f);
    // }

    if (newLevel > m_level)
    {
        m_level = newLevel;
        m_fallSpeed = std::max(0.08f, m_fallSpeed * 0.9f);
    }
}

inline void Game::handleInput(sf::Keyboard::Key key)
{
    if (isGameOver())
    {
        if (key == sf::Keyboard::Key::R)
        {
            reset();
        }
        return;
    }
    switch (key)
    {
        case sf::Keyboard::Key::Left:
        {
            movePiece(-1, 0);
            break;
        }
        case sf::Keyboard::Key::Right:
        {
            movePiece(1, 0);
            break;
        }
        case sf::Keyboard::Key::Down:
        {
            movePiece(0, 1);
            break;
        }
        case sf::Keyboard::Key::Up:
        {
            rotatePiece();
            break;
        }
        case sf::Keyboard::Key::Space:
        {
            hardDrop();
            break;
        }
        case sf::Keyboard::Key::R:
        {
            reset();
            break;
        }
        case sf::Keyboard::Key::X:
        {
            toggleMusic();
            break;
        }
        default:
        {
            break;
        }
    }
}

inline void Game::update()
{
    if (isGameOver())
    {
        return;
    }
    if (m_clock.getElapsedTime().asSeconds() > m_fallSpeed)
    {
        if (!movePiece(0, 1))
        {
            m_board.lockPiece(m_currentPiece);
            int lines = m_board.clearLines();
            int blocksCleared = lines * Board::WIDTH;  // 每行有WIDTH个方块
            m_linesCleared += lines;  // 累计消除行数
            m_score += blocksCleared;
            checkHighScore();  // 检查是否打破记录
            int newLevel = m_linesCleared / 5;  // 每5行升1级
            // m_score += m_board.clearLines() * 100;
            spawnPiece();

            // 加快下落速度
            // if (m_linesCleared > 0 && m_linesCleared % 10 == 0)
            // {
            //     m_fallSpeed = std::max(0.1f, m_fallSpeed - 0.05f);
            // }

            // if (m_linesCleared > 0 && m_linesCleared % 10 == 0)
            // {
            //     m_fallSpeed = std::max(0.05f, m_fallSpeed * 0.8f);
            // }

            if (newLevel > m_level)
            {
                m_level = newLevel;
                m_fallSpeed = std::max(0.08f, m_fallSpeed * 0.9f);
            }
        }
        m_clock.restart();
    }
}

inline void Game::draw(sf::RenderWindow& window)
{
    drawBoard(window);
    drawCurrentPiece(window);
    drawNextPiece(window);
    drawUI(window);
    if (isGameOver())
    {
        drawGameOver(window);
    }
}

inline void Game::drawBoard(sf::RenderWindow& window)
{
    const auto& m_grid = m_board.getGrid();
    for (int i = 0; i < Board::HEIGHT; i++)
    {
        for (int j = 0; j < Board::WIDTH; j++)
        {
            if (m_grid[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)])
            {
                sf::RectangleShape cell(sf::Vector2f(static_cast<float>(Board::CELL_SIZE - 1), static_cast<float>(Board::CELL_SIZE - 1)));
                cell.setPosition(sf::Vector2f(static_cast<float>(j * Board::CELL_SIZE), static_cast<float>(i * Board::CELL_SIZE)));
                cell.setFillColor(Tetromino::COLORS[static_cast<std::size_t>(m_grid[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)] - 1)]);
                window.draw(cell);
            }
        }
    }
}

inline void Game::drawCurrentPiece(sf::RenderWindow& window)
{
    for (std::size_t i = 0; i < m_currentPiece.m_shape.size(); i++)
    {
        for (std::size_t j = 0; j < m_currentPiece.m_shape[i].size(); j++)
        {
            if (m_currentPiece.m_shape[i][j])
            {
                int x = m_currentPiece.m_x + static_cast<int>(j);
                int y = m_currentPiece.m_y + static_cast<int>(i);
                if (y >= 0)
                {
                    sf::RectangleShape cell(sf::Vector2f(static_cast<float>(Board::CELL_SIZE - 1), static_cast<float>(Board::CELL_SIZE - 1)));
                    cell.setPosition(sf::Vector2f(static_cast<float>(x * Board::CELL_SIZE), static_cast<float>(y * Board::CELL_SIZE)));
                    cell.setFillColor(m_currentPiece.m_color);
                    window.draw(cell);
                }
            }
        }
    }
}

inline void Game::drawNextPiece(sf::RenderWindow& window)
{
    // 绘制下一个方块预览区域边框
    sf::RectangleShape previewArea(sf::Vector2(150.0f, 150.0f));
    previewArea.setPosition(sf::Vector2(static_cast<float>(Board::WIDTH * Board::CELL_SIZE + 30), 30.0f));
    previewArea.setFillColor(sf::Color::Transparent);
    previewArea.setOutlineColor(sf::Color::Transparent);
    previewArea.setOutlineThickness(2.0f);
    window.draw(previewArea);
    
    // 绘制Next文字
    if (m_fontLoaded)
    {
        sf::Text nextText(m_font, "Next", 24);
        nextText.setFillColor(sf::Color::White);
        nextText.setPosition(sf::Vector2f(static_cast<float>(Board::WIDTH * Board::CELL_SIZE + 15), 15.0f));
        window.draw(nextText);

        // 绘制下一个方块
        for (std::size_t i = 0; i < m_nextPiece.m_shape.size(); i++)
        {
            for (std::size_t j = 0; j < m_nextPiece.m_shape[i].size(); j++)
            {
                if (m_nextPiece.m_shape[i][j])
                {
                    sf::RectangleShape cell(sf::Vector2f(static_cast<float>(Board::CELL_SIZE - 2), static_cast<float>(Board::CELL_SIZE - 2)));
                    cell.setPosition(sf::Vector2f(static_cast<float>(Board::WIDTH * Board::CELL_SIZE + 50 + static_cast<int>(j) * Board::CELL_SIZE), static_cast<float>(80 + static_cast<int>(i) * Board::CELL_SIZE)));
                    cell.setFillColor(m_nextPiece.m_color);
                    window.draw(cell);
                }
            }
        }
    }
}

inline void Game::drawGameOver(sf::RenderWindow& window)
{
    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(Board::WIDTH * Board::CELL_SIZE), static_cast<float>(Board::HEIGHT * Board::CELL_SIZE)));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);
    if (m_fontLoaded)
    {
        sf::Text gameOverText(m_font, "Game Over!\nPress R to restart", 36);
        gameOverText.setFillColor(sf::Color::White);
        gameOverText.setPosition(sf::Vector2f(static_cast<float>(Board::WIDTH * Board::CELL_SIZE) / 2.0f - 100.0f, static_cast<float>(Board::HEIGHT * Board::CELL_SIZE) / 2.0f - 30.0f));
        window.draw(gameOverText);
    }
}

inline void Game::reset()
{
    m_board.reset();
    m_score = 0;
    m_linesCleared = 0;
    m_fallSpeed = 0.5f;
    spawnPiece();
}

inline bool Game::isGameOver() const
{
    return m_board.isGameOver();
}

inline void Game::loadHighScore()
{
    std::ifstream fin("assets/highscore.txt");
    if (fin.is_open())
    {
        fin >> m_highScore;
        fin.close();
    }
}

inline void Game::saveHighScore()
{
    #ifdef _WIN32

    system("mkdir assets 2>nul");  // 确保assets目录存在

    #endif

    std::ofstream fout("assets/highscore.txt", std::ios::out);
    if (fout.is_open())
    {
        fout << m_highScore;
        fout.close();
    }
}

inline void Game::checkHighScore()
{
    if (m_score > m_highScore && !m_newHighScore)
    {
        m_newHighScore = true;
        m_highScore = m_score;  // 立即更新显示，但先不保存
    }
}

inline void Game::toggleMusic()
{
    if (m_musicLoaded)
    {
        auto status = m_backgroundMusic.getStatus();
        if (status == sf::Music::Status::Playing)
        {
            m_backgroundMusic.pause();
        }
        else
        {
            m_backgroundMusic.play();  // 从暂停位置继续播放
        }
    }
}