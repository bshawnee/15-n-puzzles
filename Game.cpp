//
// Created by Илья Рожнев on 06.06.2022.
//

#include "Game.h"
#include <iostream>

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

void Game::interact(sf::Vector2i pos)
{
    sf::Vector2i numButton(getInteractButton(pos));
    if (numButton.x == -1 && numButton.y == -1) {
        return;
    }
    buttons_[numButton.x][numButton.y]->getText().setString("S");
    Direction direction = canBeMoved(numButton);

    switch (direction) {
        case Direction::Down:
            buttons_[numButton.x][numButton.y].swap(buttons_[numButton.x + 1][numButton.y]);
            break;
        case Direction::Up:
            buttons_[numButton.x][numButton.y].swap(buttons_[numButton.x - 1][numButton.y]);
            break;
        case Direction::Left:
            buttons_[numButton.x][numButton.y].swap(buttons_[numButton.x][numButton.y - 1]);
            break;
        case Direction::Right:
            buttons_[numButton.x][numButton.y].swap(buttons_[numButton.x][numButton.y + 1]);
            break;
        case Direction::Undefined:
            break;
        }
}

sf::Vector2i Game::getInteractButton(sf::Vector2i& pos) const
{
    for (int i = 0; i < x15; i++) {
        for (int j = 0; j < y15; j++) {
            if (buttons_[i][j] != nullptr) {
                sf::Vector2f buttonPos = buttons_[i][j]->getPosition();
                if (buttonPos.x <= pos.x && pos.x <= buttonPos.x + buttonSize &&
                    buttonPos.y <= pos.y && pos.y <= buttonPos.y + buttonSize) {
                    return {i, j};
                }
            }
        }
    }
    return {-1, -1};
}

Direction Game::canBeMoved(sf::Vector2i& button) const
{
    Direction possibleMoves[4] = { Direction::Undefined };
    if (button.x < x15 - 1) possibleMoves[0] = Direction::Down;
    if (button.x > 0) possibleMoves[1] = Direction::Up;
    if (button.y < y15 - 1) possibleMoves[2] = Direction::Right;
    if (button.y > 0) possibleMoves[3] = Direction::Left;

    for (auto possibleMove : possibleMoves) {
        switch (possibleMove) {
            case Direction::Down:
                if (buttons_[button.x + 1][button.y] == nullptr) return Direction::Down;
                break;
            case Direction::Up:
                if (buttons_[button.x - 1][button.y] == nullptr) return Direction::Up;
                break;
            case Direction::Right:
                if (buttons_[button.x][button.y + 1] == nullptr) return Direction::Right;
                break;
            case Direction::Left:
                if (buttons_[button.x][button.y - 1] == nullptr) return Direction::Left;
                break;
            case Direction::Undefined:
                break;
        }
    }
    return Direction::Undefined;
}
