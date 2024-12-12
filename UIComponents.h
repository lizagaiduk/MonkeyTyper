#pragma once
#include <SFML/Graphics.hpp>

class UIComponents {
public:
    UIComponents();
    static sf::Font fontMain;
    static sf::RectangleShape rectCreating(sf::RectangleShape& rect, float x, float y, float a, float b, const sf::Color& color, const sf::Color& outline, int thickness);

    static sf::Text
    textCreating(sf::Text &text, const std::string &string, int size, const sf::Color &color, const sf::Color &outline,
                 int thickness, float x, float y);
};


