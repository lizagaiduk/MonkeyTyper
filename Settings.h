#include <vector>
#include "SFML/Graphics.hpp"

class Settings {
public:
    std::vector<sf::Font> fonts;
    std::vector<float> speeds;
    int currentFontIndex;
    int currentSpeedIndex=0;

    Settings();
    void setCurrentFontIndex(int index);
    void setCurrentSpeedIndex(int index);
    sf::Font& getCurrentFont();
    float getCurrentSpeed();
    void nextSpeed();
    void previousSpeed();
    int getCurrentSpeedIndex() const;
};
