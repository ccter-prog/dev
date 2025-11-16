#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
    // 创建游戏窗口
    sf::RenderWindow window(sf::VideoMode({800, 700}), "Tetris");
    window.setFramerateLimit(60);

    // 初始化游戏实例
    Game game;

    // 主游戏循环
    while (window.isOpen())
    {
        while (std::optional event = window.pollEvent())
        {
            if (event -> is<sf::Event::Closed>())
            {
                game.saveHighScore();
                window.close();
            }
            else if (event -> is<sf::Event::KeyPressed>())
            {
                const auto& keyEvent = event -> getIf<sf::Event::KeyPressed>();
                if (keyEvent)
                {
                    game.handleInput(keyEvent -> code);
                }
            }
        }

        // 更新游戏状态
        game.update();

        // 渲染
        window.clear(sf::Color::Black);
        game.draw(window);
        window.display();
    }

    return 0;
}