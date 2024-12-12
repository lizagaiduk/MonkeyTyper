

#include <iostream>
#include "Settings.h"

Settings::Settings(): currentFontIndex(0), currentSpeedIndex(0) {
    sf::Font font1, font2, font3, font4;
    font1.loadFromFile("../fonts/wordFont1.ttf");
    font2.loadFromFile("../fonts/wordFont2.ttf");
    font3.loadFromFile("../fonts/wordFont3.ttf");
    fonts.push_back(font1);
    fonts.push_back(font2);
    fonts.push_back(font3);
    for (float s = 0.3f; s <= 10.0f; s += 0.1f) {
        speeds.push_back(s);
    }
}

void Settings::setCurrentFontIndex(int index) {
    if (index >= 0 && index < fonts.size()) {
        currentFontIndex = index;
    } else {
        std::cout << "Invalid font index." << std::endl;
    }
    }


void Settings::setCurrentSpeedIndex(int index) {
    if (index >= 0 && index < speeds.size()) {
        currentSpeedIndex = index;
    } else {
        std::cerr << "Invalid speed index." << std::endl;
    }
}

sf::Font &Settings::getCurrentFont() {
    if (currentFontIndex >= 0 && currentFontIndex < fonts.size()) {
        return fonts[currentFontIndex];
    } else {
        return fonts[0];
    }
}

float Settings::getCurrentSpeed() {
    return speeds[currentSpeedIndex];

}

void Settings::nextSpeed() {
    currentSpeedIndex = (currentSpeedIndex + 1) % speeds.size();

}

void Settings::previousSpeed() {
    currentSpeedIndex = (currentSpeedIndex - 1 + speeds.size()) % speeds.size();

}

int Settings::getCurrentSpeedIndex() const {
    return currentSpeedIndex;
}
