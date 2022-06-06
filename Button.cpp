//
// Created by Илья Рожнев on 06.06.2022.
//

#include "Button.h"

Button::Button(const Button& ref)
: sf::RectangleShape(ref)
, text_(ref.text_)
, font_(ref.font_)
{
    init();
}

Button::Button(Button&& ref) noexcept
: sf::RectangleShape(ref)
, text_(std::move(ref.text_))
, font_(ref.font_)
{
    init();
}

Button::Button(sf::Font& font, const std::string& text)
: font_(font)
{
    init(text);
}


void Button::init(const std::string& text /*= ""*/)
{
    text_.setFont(font_);
    if (!text.empty()) {
        text_.setString(text);
    }
    text_.setCharacterSize(buttonSize / 2);
    text_.setStyle(sf::Text::Bold);
    setSize({static_cast<float>(buttonSize), static_cast<float>(buttonSize)});
    setFillColor(sf::Color(0xE2DDDD));
    setOutlineColor(sf::Color(0x7C7C7C));
    setOutlineThickness(-12.0f);

}

void Button::setPos(const sf::Vector2f& pos)
{
    setPosition(pos);
    text_.setPosition(pos.x + buttonSize / 2 - 32, pos.y + 32);
}

sf::Text& Button::getText() {
    return text_;
}
