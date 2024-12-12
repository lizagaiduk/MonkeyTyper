#include <iostream>
#include "UIComponents.h"
sf::Font UIComponents::fontMain;

sf::Text UIComponents::textCreating(sf::Text& text, const std::string& string, int size, const sf::Color& color, const sf::Color& outline, int thickness, float x, float y) {
    text.setString(string);
    text.setFont(fontMain);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setOutlineColor(outline);
    text.setOutlineThickness(thickness);
    text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
    text.setPosition(x,y);
    return text;
}

sf::RectangleShape UIComponents::rectCreating(sf::RectangleShape& rect, float x, float y, float a, float b, const sf::Color& color, const sf::Color& outline, int thickness) {
    rect.setSize({a, b});
    rect.setOrigin(rect.getLocalBounds().width/2, rect.getLocalBounds().height/2);
    rect.setPosition({x, y});
    rect.setFillColor(color);
    rect.setOutlineColor(outline);
    rect.setOutlineThickness(thickness);
    return rect;
}

