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
private:
    std::unique_ptr<Button> buttons_[x15][y15];
    sf::Font font_;
};


#endif //INC_15_GAME_H
