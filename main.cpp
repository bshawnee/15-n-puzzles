#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
constexpr std::size_t OutlineThickness = 12;


int main() {
    sf::RenderWindow window(sf::VideoMode(y15 * buttonSize, x15 * buttonSize), "15", 7);
    window.setFramerateLimit(60);

    Game game(0, 0);
    unsigned counterPress = 0;
    while (window.isOpen()) {
        sf::Event windowEvent; // init in windowPollEvent
        window.clear();
        if (window.pollEvent(windowEvent) && windowEvent.type == sf::Event::Closed) {
            window.close();
            break;
        }
        if (!game.goingAnimation() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            game.interact(sf::Mouse::getPosition(window));
        }
        game.render(window);
        window.display();
    }
    return 0;
}
