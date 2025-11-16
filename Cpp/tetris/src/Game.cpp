#include "Game.h"
#include "Board.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <algorithm>

void Game::drawUI(sf::RenderWindow& window)
{
    if (m_fontLoaded)
    {
        float uiStartX = static_cast<float>(Board::WIDTH * Board::CELL_SIZE + 30);
        float currentY = 150.0f;  // 起始Y位置
        float lineHeight = 35.0f;  // 每行高度

        // 绘制最高分，如果有新纪录则高亮显示
        sf::Text highScoreText(m_font, "High Score: " + std::to_string(m_highScore), 24);
        highScoreText.setFillColor(m_newHighScore ? sf::Color::Yellow : sf::Color::White);
        highScoreText.setPosition(sf::Vector2f(uiStartX, currentY));
        window.draw(highScoreText);
        currentY += lineHeight;  // 下移一行

        // 如果有新纪录，显示提示
        if (m_newHighScore)
        {
            sf::Text newRecordText(m_font, "NEW RECORD!", 20);
            newRecordText.setFillColor(sf::Color::Red);
            newRecordText.setPosition(sf::Vector2f(uiStartX, currentY));
            window.draw(newRecordText);
            currentY += lineHeight;  // 下移一行
        }

        // 绘制分数
        sf::Text scoreText(m_font, "Score: " + std::to_string(m_score), 24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(sf::Vector2f(uiStartX, currentY));
        window.draw(scoreText);
        currentY += lineHeight;  // 下移一行

        // 绘制消除行数
        sf::Text linesText(m_font, "Lines: " + std::to_string(m_linesCleared), 24);
        linesText.setFillColor(sf::Color::Yellow);
        linesText.setPosition(sf::Vector2f(uiStartX, currentY));
        window.draw(linesText);
        currentY += lineHeight;  // 下移一行

        // 绘制等级
        int level = std::max(0, static_cast<int>((0.5f - m_fallSpeed) / 0.05f));
        sf::Text levelText(m_font, "Level: " + std::to_string(level), 20);
        levelText.setFillColor(sf::Color::Green);
        levelText.setPosition(sf::Vector2f(uiStartX, currentY));
        window.draw(levelText);
        currentY += lineHeight * 1.5f;  // 空一行半

        // 绘制音乐状态，修正状态检查
        std::string musicStatus;
        sf::Color musicColor;
        if (m_musicLoaded)
        {
            auto status = m_backgroundMusic.getStatus();
            if (status == sf::Music::Status::Playing)
            {
                musicStatus = "ON (X)";
                musicColor = sf::Color::Green;
            }
            else
            {
                musicStatus = "OFF (X)";
                musicColor = sf::Color::Red;
            }
        }
        sf::Text musicText(m_font, "Music: " + musicStatus, 16);
        musicText.setFillColor(musicColor);
        musicText.setPosition(sf::Vector2f(uiStartX, currentY));
        window.draw(musicText);
        currentY += lineHeight;

        // 绘制操作说明
        sf::Text controlsText(m_font, "Controls:\n←→: Move\n↑: Rotate\n↓: Speed\nSpace: Hard Drop\nR: Restart", 18);
        controlsText.setFillColor(sf::Color::White);
        controlsText.setPosition(sf::Vector2f(uiStartX, currentY));
        window.draw(controlsText);
    }
}