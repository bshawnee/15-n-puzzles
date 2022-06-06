//
// Created by Илья Рожнев on 06.06.2022.
//

#ifndef INC_15_BUTTON_H
#define INC_15_BUTTON_H
#include <SFML/Graphics.hpp>
#include "settings.h"

enum class Direction {
    Left,
    Right,
    Up,
    Down
};

class Button : public sf::RectangleShape
{
public:
    Button() = delete;
    Button(const Button& ref);
    Button(Button&& ref) noexcept ;
    Button(sf::Font& font, const std::string& text);
    ~Button() override = default;
    void setPos(const sf::Vector2f& pos);
    sf::Text& getText();
private:
    void init(const std::string& text = "");
    sf::Text text_;
    const sf::Font& font_;
};


#endif //INC_15_BUTTON_H
