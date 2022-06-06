//
// Created by Илья Рожнев on 06.06.2022.
//

#include "Game.h"

Game::Game(unsigned int /*xCountButtons*/, unsigned int /*yCountButtons*/)
{
    font_.loadFromFile("../font.ttf");
    int counter = 1;
    float yPos = 0;
    for (int x = 0; x < x15; x++) {

        float xPos = 0;

        for (int y = 0; y < y15; y++) {

            if (counter != x15 * y15) {
                buttons_[x][y] = std::make_unique<Button>(font_, std::to_string(counter));
                buttons_[x][y]->setPos({xPos, yPos});
            }
            counter++;
            xPos += buttonSize;
        }
        yPos += buttonSize;

    }
}

void Game::render(sf::RenderWindow& target)
{
    target.clear(sf::Color(19, 97, 100));
    for (int i = 0; i < x15; i++) {
        for (int j = 0; j < y15; j++) {
            if (buttons_[i][j] != nullptr) {
                target.draw(*buttons_[i][j]);
                target.draw(buttons_[i][j]->getText());
            }
        }
    }
}
