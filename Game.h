//
// Created by Илья Рожнев on 06.06.2022.
//

#ifndef INC_15_GAME_H
#define INC_15_GAME_H
#include "Button.h"

class Game
{
public:
    Game() = delete;
    Game(const Game& ref) = delete;
    Game(const Game&& ref) = delete;
    Game(unsigned xCountButtons, unsigned yCountButtons);
    ~Game() = default;

    void render(sf::RenderWindow& target);
    void interact(sf::Vector2i pos);
private:
    std::unique_ptr<Button> buttons_[x15][y15];
    sf::Font font_;
    sf::Vector2i getInteractButton(sf::Vector2i& pos) const;
    Direction canBeMoved(sf::Vector2i& button) const;
};


#endif //INC_15_GAME_H
