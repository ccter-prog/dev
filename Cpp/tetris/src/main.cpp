#include <SFML/Graphics.hpp>

int main() {
    // 使用英文标题
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My Tetris!");
    
    // 创建一个矩形方块
    sf::RectangleShape block({100.f, 100.f});
    block.setPosition({100.f, 100.f});
    block.setFillColor(sf::Color::Red);

    // 游戏循环
    while (window.isOpen()) {
        // 事件处理
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // 渲染
        window.clear();
        window.draw(block);
        window.display();
    }

    return 0;
}