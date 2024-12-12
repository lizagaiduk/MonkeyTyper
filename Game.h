#pragma once
#include <fstream>
#include <set>
#include "SFML/Graphics.hpp"
#include "SFML/Audio/Music.hpp"
#include "Settings.h"
class Game {
public:
    Game();
    void run();
    struct ScoreEntry {
        std::string playerName;
        int score;
        bool operator<(const ScoreEntry& other)const;
    };

private:
    std::vector<ScoreEntry> leaderboard;
    Settings settings;
    sf::Music menuMusic,gameMusic,gameOverMusic;
    sf::Font fontWords;
    sf::RenderWindow gameWindow;
    std::vector<std::string> words;
    sf::Clock gameClock,clockWords;
    sf::Event event;
    float interval=4;
    float speed{};
    int trial=5;
    int score=0;
    sf::Clock pausedClock;
    std::string userInput,playerName;
    sf::Time pauseTime,pauseOffset,initialElapsedTime,totalPauseDuration,elapsedTimeOffset;
    std::vector<sf::Text> screenWords;
    std::set<std::string>inputWords;
    void loadWords(const std::string &filePath);
    void detect(sf::Event&event);
    void scoreDisplaying();
    bool lose=false, highScoresOpened=false,match = false,
    inMenu=true,paused=false,settingsPressed=false,nameEntering=false;
    sf::Text scoreDisplay,trialsDisplay,highScoresLogo,
    timerDisplay,speedDisplay,gameOverText,inputDisplay,
    restartText,exitText,menuText,mainMenuText, gameOverLogo,
    newGameText,highScoresText,continueText,settingsText,menuExitText,
    pauseContinue,pauseText,pauseSaveGame,pauseMenu,pauseRestart,pause,
    font1,font2,font3,fontSettingsText,speedSettingsText,fontChangeText,
    speedIncrease,speedDecrease,wellDoneText,settingsTextSettings,nameEntryText,
    nameTextLogo,playerNameDisplay,playerNameText,finalScoreText,finalTimeText,exitHighScoresText,playerNameHS,scoreHS;
    sf::RectangleShape topLine,gameOverBackground,buttonPause,inputBox,restartButton,
    exitButton,menuButton,result,newGameRect,exitRect,continueRect,highScoresRect,
    settingsRect,pauseRect,saveRect,continueRectPause,restartButtonPause,menuButtonPause,fontChangeButton,increaseButton,
    decreaseButton,wellDoneButton,nameEntryBox,exitHighScoresRect;
    sf::Sprite monkey1,monkey2,monkey3;
    std::vector<sf::Vector2f> pos;
    std::vector<sf::Sprite> bananas;
    sf::Texture bananaTexture,monkeyTexture1,monkeyTexture2,monkeyTexture3;
    void gameOver();
    void speedDisplaying();
    void mistakesDisplaying();
    void updateTimerDisplay();
    void restartGame();
    void mouseClicksGame(float x,float y);
    void pauseCreation();
    void pauseDraw();
    void gamePause();
    void resumeGame();
    void settingsCreation();
    void settingsDraw();
    void mouseClicksPause(float x, float y);
    void applySettings();
    void mouseClicksSettings(float x, float y);
    void drawNameBox();
    void enterOfTheName(sf::Event &event);
    void saveGameState();
    void loadGameState();
    void highScoresCreation();
    void drawHighScores();
    void mouseClicksHighScores(float x, float y);
    void loadLeaderboard();
    void saveLeaderboard();
    void updateLeaderboard(const std::string &playerName, int score);
    void drawGame();
    void wordsGenerationAndMoving();
    void drawMenu();
    void menuCreation();
    void mouseClicksGameOver(float x, float y);
    void drawGameOver();
    void mouseClicksMenu(float x, float y);
};


