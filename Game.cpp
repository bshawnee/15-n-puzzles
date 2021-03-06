//
// Created by Илья Рожнев on 06.06.2022.
//

#include "Game.h"
#include <iostream>
#include <array>

int Game::y15 = 3;
int Game::x15 = 3;

Game::Game(const std::vector<int>& map)
{
    buttons_.resize(Game::x15);
    for (auto& button : buttons_) {
        button.resize(Game::y15);
    }
    soundBuffer_.loadFromFile("../sound.wav");
    sound_.setBuffer(soundBuffer_);
    font_.loadFromFile("../font.ttf");
    success_.setFont(font_);
    success_.setPosition(x15 * buttonSize / 4, y15 * buttonSize / 3);
    success_.setString("congratulations!");
    success_.setStyle(sf::Text::Bold);
    success_.setFillColor(sf::Color(250, 191, 191));
    success_.setOutlineColor(sf::Color::Black);
    success_.setCharacterSize(100);
    int counter = 1;
    float yPos = 0;
    for (int x = 0; x < x15; x++) {

        float xPos = 0;

        for (int y = 0; y < y15; y++) {

            if (map[counter - 1] != 0) {
                buttons_[x][y] = std::make_unique<Button>(font_, std::to_string(map[counter - 1]));
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
    if (gameIsOver()) {
        target.draw(success_);
        return;
    }
    target.clear(sf::Color(19, 97, 100));
    for (int i = 0; i < x15; i++) {
        for (int j = 0; j < y15; j++) {
            if (buttons_[i][j] != nullptr) {
                if (needAnimation_.first != Direction::Undefined && needAnimation_.second != nullptr) {
                    if (needAnimation_.second == buttons_[i][j].get()) {

                        switch (needAnimation_.first) {
                            case Direction::Right:
                                buttons_[i][j]->move(animationSpeed, 0);
                                buttons_[i][j]->getText().move(animationSpeed, 0);
                                break;
                            case Direction::Left:
                                buttons_[i][j]->move(animationSpeed * -1.0f, 0);
                                buttons_[i][j]->getText().move(animationSpeed * -1.0f, 0);
                                break;
                            case Direction::Down:
                                buttons_[i][j]->move(0, animationSpeed);
                                buttons_[i][j]->getText().move(0, animationSpeed);
                                break;
                            case Direction::Up:
                                buttons_[i][j]->move(0, animationSpeed * -1.0f);
                                buttons_[i][j]->getText().move(0, animationSpeed * -1.0f);
                                break;
                            default:
                                break;

                        }

                        switch (needAnimation_.first) {
                            case Direction::Right:
                                if (buttons_[i][j]->getPosition().x >= prevPosition_.x + static_cast<float>(buttonSize)) {
                                    needAnimation_.first = Direction::Undefined;
                                    needAnimation_.second = nullptr;
                                }
                                break;
                            case Direction::Left:
                                if (buttons_[i][j]->getPosition().x <= prevPosition_.x - static_cast<float>(buttonSize)) {
                                    needAnimation_.first = Direction::Undefined;
                                    needAnimation_.second = nullptr;
                                }
                                break;
                            case Direction::Down:
                                if (buttons_[i][j]->getPosition().y >= prevPosition_.y + static_cast<float>(buttonSize)) {
                                    needAnimation_.first = Direction::Undefined;
                                    needAnimation_.second = nullptr;
                                }
                                break;
                            case Direction::Up:
                                if (buttons_[i][j]->getPosition().y <= prevPosition_.y - static_cast<float>(buttonSize)) {
                                    needAnimation_.first = Direction::Undefined;
                                    needAnimation_.second = nullptr;
                                }
                                break;
                            default:
                                break;
                        }
                        if (needAnimation_.first == Direction::Undefined && needAnimation_.second == nullptr) {
                            sound_.play();
                            if (gameIsOver()) {
                                target.draw(success_);
                                needUpdate_ = false;
                            }
                        }
                    }
                }
                target.draw(*buttons_[i][j]);
                target.draw(buttons_[i][j]->getText());
            }
        }
    }
}

void Game::interact(sf::Vector2i pos)
{
    if (!needUpdate()) return;

    sf::Vector2i numButton(getInteractButton(pos));
    if (numButton.x == -1 && numButton.y == -1) {
        return;
    }
    Direction direction = canBeMoved(numButton);
    if (direction != Direction::Undefined) {
        needAnimation_.first = direction;
        needAnimation_.second = buttons_[numButton.x][numButton.y].get();

        prevPosition_ = buttons_[numButton.x][numButton.y]->getPosition();
    }
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
            needAnimation_.first = Direction::Undefined;
            needAnimation_.second = nullptr;
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
    std::array<Direction, 4> possibleMoves{};

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

bool Game::goingAnimation() const
{
    if (needAnimation_.first != Direction::Undefined && needAnimation_.second != nullptr) {
        return true;
    }
    return false;
}

bool Game::gameIsOver() const
{
    int prevNum = -1;

    for (int i = 0; i < x15; i++) {
        for (int j = 0; j < y15; j++) {
            if (buttons_[i][j] != nullptr) {
                int tmp = std::stoi(buttons_[i][j]->getText().getString().operator std::string());
                if (prevNum == -1) {
                    prevNum = tmp;
                    continue;
                }
                if (prevNum + 1 == tmp) {
                    prevNum = tmp;
                    continue;
                }
                else {
                    return false;
                }
            }
            if (prevNum == x15 * y15 - 1 && buttons_[i][j] == nullptr && i == x15 - 1 && j == y15 - 1) {
                return true;
            }
        }
    }
    return true;
}

bool Game::needUpdate() const
{
    return needUpdate_;
}
