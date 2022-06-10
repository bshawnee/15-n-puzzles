#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include <unistd.h>
constexpr std::size_t OutlineThickness = 12;


int main() {
    sf::RenderWindow window(sf::VideoMode(y15 * buttonSize, x15 * buttonSize), "15", 7);
    window.setFramerateLimit(60);
    Game game(0, 0);
    while (window.isOpen()) {
        sf::Event windowEvent;
        window.clear();
        if (window.pollEvent(windowEvent) && windowEvent.type == sf::Event::Closed) {
            window.close();
            break;
        }
        if (!game.goingAnimation() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            game.interact(sf::Mouse::getPosition(window));
        }
        game.render(window);
        if (game.needUpdate())
        window.display();
    }
    return 0;
}
