//
// Created by Илья Рожнев on 06.06.2022.
//

#ifndef INC_15_GAME_H
#define INC_15_GAME_H
#include "Button.h"
#include <SFML/Audio.hpp>
class Game
{
public:
    Game() = delete;
    Game(const Game& ref) = delete;
    Game(const Game&& ref) = delete;
    Game(const std::vector<int>& map);
    ~Game() = default;

    void render(sf::RenderWindow& target);
    void interact(sf::Vector2i pos);
    bool goingAnimation() const;
    bool gameIsOver() const;
    bool needUpdate() const;
    static int x15;
    static int y15;
private:
    sf::Text success_;
    bool needUpdate_ = true;
    std::pair<Direction, Button*> needAnimation_;
    sf::Vector2f prevPosition_;
    std::vector<std::vector<std::unique_ptr<Button>>> buttons_;
    sf::Font font_;
    sf::Vector2i getInteractButton(sf::Vector2i& pos) const;
    Direction canBeMoved(sf::Vector2i& button) const;
    sf::SoundBuffer soundBuffer_;
    sf::Sound sound_;
};


#endif //INC_15_GAME_H
