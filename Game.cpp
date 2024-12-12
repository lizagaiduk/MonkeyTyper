#include <iostream>
#include <fstream>
#include "Game.h"
#include "UIComponents.h"
#include <algorithm>
#include <random>
#include <iomanip>
#include <SFML/Audio.hpp>

Game::Game(){
    gameWindow.create(sf::VideoMode::getDesktopMode(),"Monkey Typer", sf::Style::Fullscreen,
                      sf::ContextSettings(0, 0, 8));
    if (!UIComponents::fontMain.loadFromFile("../fonts/KnightWarrior.ttf")) {
        std::cout << "Error while loading font!";
    }
    if (!fontWords.loadFromFile("../fonts/wordFont1.ttf")){
        std::cout << "Error while loading font!";
    }
    UIComponents::textCreating(scoreDisplay,"Score: 0 ", 70, sf::Color::Green, sf::Color::Black, 4, 2200, 100);
    UIComponents::textCreating(trialsDisplay,"Trials: 5 ", 70, sf::Color::Yellow, sf::Color::Black, 4, 300, 100);
    UIComponents::textCreating(timerDisplay,"Time: 00:00 ", 70, sf::Color::Red, sf::Color::Black, 4, 1580, 100);
    UIComponents::rectCreating(topLine,gameWindow.getSize().x/2,gameWindow.getSize().y/12,gameWindow.getSize().x,gameWindow.getSize().y/6,sf::Color::White,sf::Color::Black,4);
    UIComponents::textCreating(inputDisplay,"",30,sf::Color::Green,sf::Color::Black,2, gameWindow.getSize().x / 2-80, gameWindow.getSize().y-60);
    UIComponents::rectCreating(inputBox,gameWindow.getSize().x/2,gameWindow.getSize().y-50,300,100,sf::Color::White,sf::Color::Black,4);
    UIComponents::textCreating(pause,"Pause",70,sf::Color::Magenta,sf::Color::Black,4,1000,100);
    UIComponents::rectCreating(pauseRect,1000,120,210,90,sf::Color::White,sf::Color::White,4);
    UIComponents::rectCreating(nameEntryBox, gameWindow.getSize().x / 2-50, gameWindow.getSize().y / 2+90, 600, 200, sf::Color::White, sf::Color::Magenta, 5);
    UIComponents::textCreating(playerNameDisplay, "", 100, sf::Color::Green, sf::Color::Magenta, 4, gameWindow.getSize().x / 2-200, gameWindow.getSize().y / 2+25);
    UIComponents::textCreating(nameTextLogo, "Enter Your Name:", 200, sf::Color::Magenta, sf::Color::Green, 7, gameWindow.getSize().x / 2, gameWindow.getSize().y / 2 - 300);
    loadWords("../txtFiles/words1.txt");
    loadLeaderboard();
    if (!menuMusic.openFromFile("../music/menuMusic.ogg")) {
        std::cout << "Error while loading menu music!";
    }
    if (!gameMusic.openFromFile("../music/gameMusic.ogg")) {
        std::cout << "Error while loading game music!";
    }
    if (!gameOverMusic.openFromFile("../music/gameOverMusic.ogg")) {
        std::cout << "Error while loading game over music!";
    }
    menuMusic.setLoop(true);
    menuMusic.play();
}
void Game::loadWords(const std::string& filePath) {
    std::fstream file(filePath);
    if (!file.is_open()) {
        std::cout<<"Error while opening the file.";
    }
    std::string line;
    while (std::getline(file, line)) {
        words.push_back(line);
    }
    file.close();
}
void Game::wordsGenerationAndMoving() {
    if (clockWords.getElapsedTime().asSeconds() >= interval) {
        clockWords.restart();
        int randInd = rand() % words.size() - 1;
        sf::Text text(words[randInd], settings.getCurrentFont(), 50);
        text.setFillColor(sf::Color::Black);
        text.setPosition(-text.getLocalBounds().width, std::rand() % (gameWindow.getSize().y-
                                                                      static_cast<int>(topLine.getSize().y)-static_cast<int>(inputBox.getSize().y)-100)
                                                       + static_cast<int>(topLine.getSize().y));
        screenWords.push_back(text);
    }

    for (auto iter = screenWords.begin(); iter != screenWords.end();) {
        iter->move(speed, 0);
        gameWindow.draw(*iter);
        if (iter->getPosition().x > gameWindow.getSize().x) {
            iter = screenWords.erase(iter);
            trial--;
            mistakesDisplaying();
            if (trial == 0) {
                gameOver();
            }
        }else{
            iter++;
        }
    }
}
void Game::run() {
    menuCreation();
    while (gameWindow.isOpen()) {
        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
            }
            if (nameEntering) {
                enterOfTheName(event);
                continue;
            }
            if (inMenu && event.type == sf::Event::MouseButtonPressed) {
                mouseClicksMenu(event.mouseButton.x, event.mouseButton.y);
            } else if (settingsPressed && event.type == sf::Event::MouseButtonPressed) {
                mouseClicksSettings(event.mouseButton.x, event.mouseButton.y);
            }else if (highScoresOpened&&event.type == sf::Event::MouseButtonPressed){
                mouseClicksHighScores(event.mouseButton.x, event.mouseButton.y);
            }
            else if (paused && event.type == sf::Event::MouseButtonPressed) {
                mouseClicksPause(event.mouseButton.x, event.mouseButton.y);
            } else if (!lose && event.type == sf::Event::MouseButtonPressed) {
                mouseClicksGame(event.mouseButton.x, event.mouseButton.y);
            } else if (event.type == sf::Event::MouseButtonPressed) {
                mouseClicksGameOver(event.mouseButton.x, event.mouseButton.y);
            }
            if (!inMenu && !lose && !paused && !nameEntering) {
                detect(event);
            }
        }
        gameWindow.clear();
        if (inMenu && !settingsPressed) {
            drawMenu();
        } else if (settingsPressed) {
            settingsCreation();
            settingsDraw();

        } else if(highScoresOpened){
            highScoresCreation();
            drawHighScores();
        }else if (nameEntering) {
            drawNameBox();
        } else if (paused) {
            pauseCreation();
            pauseDraw();
        } else if (!lose) {
            drawGame();
            updateTimerDisplay();
            wordsGenerationAndMoving();
        } else {
            drawGameOver();

        }
        gameWindow.display();
    }
}
void Game::enterOfTheName(sf::Event &event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        nameEntering = false;
        inMenu = false;
        highScoresOpened=false;
        match=true;
        menuMusic.stop();
        gameMusic.setLoop(true);
        gameMusic.play();
        gameClock.restart();
    } else if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode >= 65 && event.text.unicode <= 122 && event.text.unicode != 8&&playerName.length()<14) {
            playerName += static_cast<char>(event.text.unicode);
            playerNameDisplay.setString(playerName);
        } else if (event.text.unicode == 8 && !playerName.empty()) {
            playerName.pop_back();
            playerNameDisplay.setString(playerName);
        }
        sf::FloatRect textRect = playerNameDisplay.getLocalBounds();
        playerNameDisplay.setOrigin(textRect.width / 2, textRect.height / 2);
        playerNameDisplay.setPosition(gameWindow.getSize().x / 2 - 50, gameWindow.getSize().y / 2 + 70);
    }
}
bool Game::ScoreEntry::operator<(const ScoreEntry& other) const {
    return score > other.score;
}
void Game::loadLeaderboard() {
    std::fstream file("../txtFiles/highScores.txt");
    leaderboard.clear();
    ScoreEntry entry;
    while (file >> entry.playerName >> entry.score) {
        leaderboard.push_back(entry);
    }
    file.close();
    std::sort(leaderboard.begin(), leaderboard.end());
    if (leaderboard.size() > 5) {
        leaderboard.resize(5);
    }
}
void Game::saveLeaderboard() {
    std::fstream file("../txtFiles/highScores.txt");
    for (const auto& entry : leaderboard) {
        file << entry.playerName << " " << entry.score << '\n';
    }
    file.close();
}
void Game::updateLeaderboard(const std::string& playerName, int score) {
    ScoreEntry newEntry { playerName, score };
    leaderboard.push_back(newEntry);
    std::sort(leaderboard.begin(), leaderboard.end());
    if (leaderboard.size() > 5) {
        leaderboard.resize(5);
    }
    saveLeaderboard();
}
void Game::saveGameState() {
    std::fstream outFile("../txtFiles/savedGame.txt");
    if (outFile.is_open()) {
        outFile << playerName << "\n";
        outFile << score << " " << trial << " " << settings.getCurrentSpeedIndex()
                << " "<<(elapsedTimeOffset+gameClock.getElapsedTime()-totalPauseDuration).asSeconds()<<" " << settings.currentFontIndex<<"\n";

        for (const auto& word : screenWords) {
            outFile << word.getString().toAnsiString() << " "
                    << word.getPosition().x << " "
                    << word.getPosition().y << "\n";
        }
        outFile.close();
    } else {
        std::cerr << "Unable to open file for saving." << std::endl;
    }
}
void Game::loadGameState() {
    std::fstream inFile("../txtFiles/savedGame.txt");
    if (inFile.is_open()) {
        if(lose){
            gameOver();
        }else{
            gameMusic.play();
            gameMusic.setLoop(true);
            std::string loadedPlayerName;
            getline(inFile, loadedPlayerName);
            playerName = loadedPlayerName;
            playerNameDisplay.setString(playerName);
            int fontIndex,speedIndex;
            float elapsedTime;
            int loadedScore,loadedTrials;
            inFile >> loadedScore >> loadedTrials>> speedIndex>>elapsedTime>> fontIndex;
            score=loadedScore;
            trial=loadedTrials;
            settings.setCurrentFontIndex(fontIndex);
            settings.setCurrentSpeedIndex(speedIndex);
            speed = settings.getCurrentSpeed();
            elapsedTimeOffset=sf::seconds(elapsedTime);
            totalPauseDuration = sf::Time::Zero;
            gameClock.restart();
            screenWords.clear();
            std::string wordText;
            float posX, posY;
            while (inFile >> wordText >> posX >> posY) {
                sf::Text word(wordText, settings.getCurrentFont(), 50);
                word.setFillColor(sf::Color::Black);
                word.setPosition(posX, posY);
                screenWords.push_back(word);
            }
            inFile.close();
            scoreDisplaying();
            mistakesDisplaying();
        }
    }
    else {
        std::cout << "Unable to open saveGame.txt for reading." << std::endl;
    }
}

void Game::detect(sf::Event&event) {
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode >= 65 && event.text.unicode <= 122 && event.text.unicode != 8) {
            userInput += static_cast<char>(event.text.unicode);
            inputDisplay.setString(userInput);
        } else if (event.text.unicode == 8 && !userInput.empty()) {
            userInput.pop_back();
            inputDisplay.setString(userInput);
        }
    } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        match = false;
        for (auto iter = screenWords.begin(); iter != screenWords.end();) {
            if (userInput == iter->getString()) {
                match = true;
                score++;
                screenWords.erase(iter);
                scoreDisplaying();
            } else {
                ++iter;
            }
        }
        if (!match) {
            trial--;
            mistakesDisplaying();
            if (trial == 0) {
                gameOver();

            }
        }
        userInput.clear();
        inputDisplay.setString(userInput);
    }
}

void Game::mouseClicksSettings(float x,float y){
    auto mousePos = sf::Mouse::getPosition(gameWindow);
    if(fontChangeButton.getGlobalBounds().contains({ static_cast<float>(mousePos.x),
                                                     static_cast<float>(mousePos.y)})){
        settings.currentFontIndex = (settings.currentFontIndex + 1) % settings.fonts.size();
    }else if (wellDoneButton.getGlobalBounds().contains({ static_cast<float>(mousePos.x),
                                                          static_cast<float>(mousePos.y)})){
        applySettings();
        settingsPressed=false;
        inMenu=true;
    }else if(increaseButton.getGlobalBounds().contains({static_cast<float>(mousePos.x),
                                                        static_cast<float>(mousePos.y)})){
        settings.nextSpeed();
        speed=settings.getCurrentSpeed();
        speedDisplaying();
    }else if(decreaseButton.getGlobalBounds().contains({static_cast<float>(mousePos.x),
                                                        static_cast<float>(mousePos.y)})){
        settings.previousSpeed();
        speed = settings.getCurrentSpeed();
        speedDisplaying();
    }
}

void Game::mouseClicksMenu(float x,float y) {
    auto mousePos = sf::Mouse::getPosition(gameWindow);
    if (newGameRect.getGlobalBounds().contains({
                                                       static_cast<float>(mousePos.x),
                                                       static_cast<float>(mousePos.y)})) {
        playerName.clear();
        playerNameDisplay.setString("");
        nameEntering=true;
        restartGame();
    } else if (exitRect.getGlobalBounds().contains({static_cast<float>(mousePos.x),
                                                    static_cast<float>(mousePos.y)})) {
        gameWindow.close();
    }else if (settingsRect.getGlobalBounds().contains({static_cast<float>(mousePos.x),
                                                       static_cast<float>(mousePos.y)})){
        settingsPressed= true;
        inMenu=false;
    }else if (continueRect.getGlobalBounds().contains({static_cast<float>(mousePos.x),
                                                       static_cast<float>(mousePos.y)})) {
        loadGameState();
        inMenu=false;
        paused=false;
        menuMusic.stop();
    }else if (highScoresRect.getGlobalBounds().contains({static_cast<float>(mousePos.x),
                                                         static_cast<float>(mousePos.y)})) {
        inMenu=false;
        paused=false;
        highScoresOpened=true;
    }
}

void Game::mouseClicksPause(float x,float y) {
    auto mousePos = sf::Mouse::getPosition(gameWindow);
    if (restartButtonPause.getGlobalBounds().contains({
                                                              static_cast<float>(mousePos.x),
                                                              static_cast<float>(mousePos.y)})){
        restartGame();
        gameMusic.stop();
        gameMusic.setLoop(true);
        gameMusic.play();
    }else if(menuButtonPause.getGlobalBounds().contains({static_cast<float>(mousePos.x),
                                                         static_cast<float>(mousePos.y)})) {
        inMenu=!inMenu;
        gameMusic.stop();
        menuMusic.setLoop(true);
        menuMusic.play();
    }else if(continueRectPause.getGlobalBounds().contains({static_cast<float>(mousePos.x),
                                                           static_cast<float>(mousePos.y)})){
        resumeGame();

    }else if (pauseSaveGame.getGlobalBounds().contains({static_cast<float>(mousePos.x),
                                                        static_cast<float>(mousePos.y)})) {
        saveGameState();
        inMenu=true;
        paused=false;
        gameMusic.stop();
        menuMusic.play();

    }
}
void Game::mouseClicksGame(float x,float y){
    auto mousePos = sf::Mouse::getPosition(gameWindow);
    if(pauseRect.getGlobalBounds().contains({static_cast<float>(mousePos.x),
                                             static_cast<float>(mousePos.y)})){
        gamePause();
    }
}
void Game::mouseClicksHighScores(float x,float y){
    auto mousePos = sf::Mouse::getPosition(gameWindow);
    if (exitHighScoresRect.getGlobalBounds().contains({
                                                              static_cast<float>(mousePos.x),
                                                              static_cast<float>(mousePos.y)})) {
        inMenu=true;
        highScoresOpened=false;
    }
}
void Game::mouseClicksGameOver(float x,float y) {
    auto mousePos = sf::Mouse::getPosition(gameWindow);
    if (exitButton.getGlobalBounds().contains({
                                                      static_cast<float>(mousePos.x),
                                                      static_cast<float>(mousePos.y)})) {
        gameWindow.close();
    }else if (restartButton.getGlobalBounds().contains({
                                                               static_cast<float>(mousePos.x),
                                                               static_cast<float>(mousePos.y)})){
        restartGame();
        gameOverMusic.stop();
        gameMusic.setLoop(true);
        gameMusic.play();
    }
    else if(menuButton.getGlobalBounds().contains({static_cast<float>(mousePos.x),
                                                   static_cast<float>(mousePos.y)})){
        inMenu=!inMenu;
        gameOverMusic.stop();
        menuMusic.setLoop(true);
        menuMusic.play();
    }
}
void Game::speedDisplaying() {
    UIComponents::textCreating(speedDisplay, "", 120, sf::Color::Green, sf::Color::Blue, 4, gameWindow.getSize().x / 2 + 200,
                               gameWindow.getSize().y / 2 + 150);
    std::ostringstream speedStream;
    speedStream << std::fixed << std::setprecision(1) << speed;
    speedDisplay.setString(speedStream.str());
}
void Game::scoreDisplaying() {
    scoreDisplay.setString("Score: " + std::to_string(score));
}
void Game::mistakesDisplaying() {
    trialsDisplay.setString("Trials: " + std::to_string(trial));
}
void Game::updateTimerDisplay() {
    sf::Time elapsed = elapsedTimeOffset + gameClock.getElapsedTime() - totalPauseDuration;
    int totalSeconds = static_cast<int>(elapsed.asSeconds());
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    std::stringstream time;
    time << "Time: "<<std::setw(2) << std::setfill('0') << minutes
         << ":" << std::setw(2) << std::setfill('0') << seconds;
    timerDisplay.setString(time.str());
}
void Game::gamePause(){
    if(!paused) {
        paused = true;
        lose = false;
        for (auto word: screenWords) {
            speed = 0;
        }
        pausedClock.restart();
    }
}
void Game::resumeGame(){
    if(paused) {
        paused = false;
        lose = false;
        for (auto word: screenWords) {
            speed = settings.getCurrentSpeed();
        }
        totalPauseDuration += pausedClock.getElapsedTime();
    }
}
void Game::applySettings() {
    for (auto &word: screenWords) {
        word.setFont(settings.getCurrentFont());
    }
    inputDisplay.setFont(settings.getCurrentFont());
    speed = settings.getCurrentSpeed();
}

void Game::restartGame() {
    score = 0;
    trial = 5;
    speed = settings.getCurrentSpeed();
    lose = false;
    inMenu= false;
    paused=false;
    screenWords.clear();
    userInput.clear();
    scoreDisplay.setString("Score: 0");
    trialsDisplay.setString("Trials: 5");
    inputDisplay.setString("");
    totalPauseDuration=sf::Time::Zero;
    elapsedTimeOffset=sf::Time::Zero;
    gameClock.restart();
    clockWords.restart();
}
void Game::gameOver() {
    updateLeaderboard(playerName, score);
    saveLeaderboard();
    lose = true;
    gameMusic.stop();
    gameOverMusic.setLoop(true);
    gameOverMusic.play();
    UIComponents::textCreating(gameOverText, "Game Over", 300, sf::Color::Yellow,
                               sf::Color::Red, 10,
                               gameWindow.getSize().x / 2, gameWindow.getSize().y / 2 - 300);
    UIComponents::rectCreating(restartButton, gameWindow.getSize().x / 2, gameWindow.getSize().y / 2 + 350, 400, 150,
                               sf::Color::Yellow, sf::Color::Red, 4);
    UIComponents::rectCreating(menuButton, gameWindow.getSize().x / 2 - 450, gameWindow.getSize().y / 2 + 350, 400, 150,
                               sf::Color::Yellow, sf::Color::Red, 4);
    UIComponents::rectCreating(exitButton, gameWindow.getSize().x / 2 + 450, gameWindow.getSize().y / 2 + 350, 400, 150,
                               sf::Color::Yellow, sf::Color::Red, 4);
    UIComponents::textCreating(restartText, "Restart", 100, sf::Color::Red, sf::Color::Black, 4, gameWindow.getSize().x / 2,
                               gameWindow.getSize().y / 2 + 320);
    UIComponents::textCreating(menuText, "Menu", 100, sf::Color::Red, sf::Color::Black, 4, gameWindow.getSize().x / 2 - 450,
                               gameWindow.getSize().y / 2 + 320);
    UIComponents::textCreating(exitText, "Exit", 100, sf::Color::Red, sf::Color::Black, 4, gameWindow.getSize().x / 2 + 450,
                               gameWindow.getSize().y / 2 + 320);
    if (!monkeyTexture3.loadFromFile("../images/monkey3.png"))
        std::cout << "Error while loading image\n";
    monkey3.setTexture(monkeyTexture3);
    monkey3.setOrigin(monkey3.getLocalBounds().width / 2, monkey3.getLocalBounds().height / 2);
    monkey3.setPosition(gameWindow.getSize().x/2,200);
    monkey3.setScale(0.7f, 0.7f);

    UIComponents::textCreating(finalScoreText, "Final Score: " + std::to_string(score), 60, sf::Color::Green, sf::Color::Blue, 4, gameWindow.getSize().x / 2, gameWindow.getSize().y / 2 - 85);
    UIComponents::textCreating(playerNameText, "Player: " + playerName, 60, sf::Color::Green, sf::Color::Blue, 4, gameWindow.getSize().x / 2, gameWindow.getSize().y / 2);
    UIComponents::textCreating(finalTimeText, timerDisplay.getString(), 60, sf::Color::Green, sf::Color::Blue, 4, gameWindow.getSize().x / 2, gameWindow.getSize().y / 2 + 100);
}
void Game::drawGameOver(){
    gameWindow.clear(sf::Color(150,255,80));
    gameWindow.draw(result);
    gameWindow.draw(restartButton);
    gameWindow.draw(restartText);
    gameWindow.draw(exitButton);
    gameWindow.draw(exitText);
    gameWindow.draw(menuButton);
    gameWindow.draw(menuText);
    gameWindow.draw(gameOverText);
    gameWindow.draw(monkey3);

    gameWindow.draw(finalScoreText);
    gameWindow.draw(playerNameText);
    gameWindow.draw(finalTimeText);
}

void Game::menuCreation(){
    UIComponents::textCreating(mainMenuText,"Monkey Typer",250,sf::Color::Yellow,
                               sf::Color::Red,10,gameWindow.getSize().x/2,gameWindow.getSize().y/2-400);
    UIComponents::textCreating(newGameText,"New game",100,sf::Color::Red,sf::Color::Yellow,5,
                               gameWindow.getSize().x/2,gameWindow.getSize().y/2-100);
    UIComponents::textCreating(highScoresText,"High Scores",80,sf::Color::Red,sf::Color::Yellow,5,
                               gameWindow.getSize().x/2,gameWindow.getSize().y/2+30);
    UIComponents::textCreating(continueText,"Continue",100,sf::Color::Red,sf::Color::Yellow,5,
                               gameWindow.getSize().x/2,gameWindow.getSize().y/2+150);
    UIComponents::textCreating(menuExitText,"Exit",100,sf::Color::Red,sf::Color::Yellow,5,
                               gameWindow.getSize().x/2,gameWindow.getSize().y/2+450);
    UIComponents::textCreating(settingsText,"Settings",100,sf::Color::Red,sf::Color::Yellow,5,
                               gameWindow.getSize().x/2,gameWindow.getSize().y/2+300);
    UIComponents::rectCreating(newGameRect,gameWindow.getSize().x/2,gameWindow.getSize().y/2-85,400,100,
                               sf::Color::Black,sf::Color::Black,4);
    UIComponents::rectCreating(highScoresRect,gameWindow.getSize().x/2,gameWindow.getSize().y/2+40,400,100,sf::Color::Black,sf::Color::Black,4);
    UIComponents::rectCreating(continueRect,gameWindow.getSize().x/2,gameWindow.getSize().y/2+170,400,100,sf::Color::Black,sf::Color::Black,4);
    UIComponents::rectCreating(exitRect,gameWindow.getSize().x/2,gameWindow.getSize().y/2+470,250,100,sf::Color::Black,sf::Color::Black,4);
    UIComponents::rectCreating(settingsRect,gameWindow.getSize().x/2,gameWindow.getSize().y/2+320,400,100,sf::Color::Black,sf::Color::Black,4);
    if (!bananaTexture.loadFromFile("../images/banana.png"))
        std::cout << "Error while loading image\n";
    pos = {{static_cast<float>(gameWindow.getSize().x / 2 - gameWindow.getSize().x / 3), static_cast<float>(gameWindow.getSize().y / 2 +
                                                                                                            gameWindow.getSize().y / 3)},
           {static_cast<float>(gameWindow.getSize().x / 2 - gameWindow.getSize().x / 3), static_cast<float>(gameWindow.getSize().y / 2 -
                                                                                                            gameWindow.getSize().y / 3)},
           {static_cast<float>( gameWindow.getSize().x / 2 + gameWindow.getSize().x/ 3), static_cast<float>(gameWindow.getSize().y / 2 +
                                                                                                            gameWindow.getSize().y / 3)},
           {static_cast<float>( gameWindow.getSize().x / 2 + gameWindow.getSize().x / 3), static_cast<float>(gameWindow.getSize().y/ 2 -
                                                                                                             gameWindow.getSize().y / 3)},
           {static_cast<float>( gameWindow.getSize().x / 2 - gameWindow.getSize().x/ 3), static_cast<float>(gameWindow.getSize().y/ 2)},
           {static_cast<float>( gameWindow.getSize().x / 2 + gameWindow.getSize().x/ 3), static_cast<float>(gameWindow.getSize().y / 2)}};
    for (const auto &p: pos) {
        sf::Sprite bananaSprite;
        bananaSprite.setTexture(bananaTexture);
        bananaSprite.setOrigin(bananaSprite.getLocalBounds().width / 2, bananaSprite.getLocalBounds().height / 2);
        bananaSprite.setPosition(p);
        bananaSprite.setScale(0.25f, 0.25f);
        bananas.push_back(bananaSprite);
    }
    if (!monkeyTexture1.loadFromFile("../images/monkey1.png"))
        std::cout << "Error while loading image\n";
    monkey1.setTexture(monkeyTexture1);
    monkey1.setOrigin(monkey1.getLocalBounds().width / 2, monkey1.getLocalBounds().height / 2);
    monkey1.setPosition(gameWindow.getSize().x/2-80,160);
    monkey1.setScale(0.5f, 0.5f);

}
void Game::drawMenu(){
    gameWindow.clear(sf::Color::Black);
    gameWindow.draw(mainMenuText);
    gameWindow.draw(newGameRect);
    gameWindow.draw(newGameText);
    gameWindow.draw(highScoresRect);
    gameWindow.draw(highScoresText);
    gameWindow.draw(continueRect);
    gameWindow.draw(continueText);
    gameWindow.draw(exitRect);
    gameWindow.draw(menuExitText);
    gameWindow.draw(settingsRect);
    gameWindow.draw(settingsText);
    for (const auto &banana: bananas) {
        gameWindow.draw(banana);
    };
    gameWindow.draw(monkey1);

}
void Game::pauseCreation(){
    UIComponents::textCreating(pauseText,"Paused!",250,sf::Color::Green,sf::Color::Blue,8,gameWindow.getSize().x/2,gameWindow.getSize().y/2-420);
    UIComponents::textCreating(pauseContinue,"Continue",100,sf::Color::Yellow,sf::Color::Red,4,gameWindow.getSize().x/2,gameWindow.getSize().y/2-150);
    UIComponents::textCreating(pauseRestart,"Restart",100,sf::Color::Magenta,sf::Color::Cyan,4,gameWindow.getSize().x/2,gameWindow.getSize().y/2);
    UIComponents::textCreating(pauseSaveGame,"Save the game",100,sf::Color::Green,sf::Color::Blue,4,gameWindow.getSize().x/2,gameWindow.getSize().y/2+150);
    UIComponents::textCreating(pauseMenu,"Back to the menu",100,sf::Color::Red,sf::Color::Yellow,4,gameWindow.getSize().x/2,gameWindow.getSize().y/2+300);
    UIComponents::rectCreating(continueRectPause,gameWindow.getSize().x/2,gameWindow.getSize().y/2-130,600,90,sf::Color(100,200,255),sf::Color::White,0);
    UIComponents::rectCreating(restartButtonPause,gameWindow.getSize().x/2,gameWindow.getSize().y/2+20,600,90,sf::Color(100,200,255),sf::Color::White,0);
    UIComponents::rectCreating(saveRect,gameWindow.getSize().x/2,gameWindow.getSize().y/2+170,600,120,sf::Color(100,200,255),sf::Color::White,0);
    UIComponents::rectCreating(menuButtonPause,gameWindow.getSize().x/2,gameWindow.getSize().y/2+320,700,120,sf::Color(100,200,255),sf::Color::White,0);

    if (!monkeyTexture2.loadFromFile("../images/monkey2.png"))
        std::cout << "Error while loading image\n";
    monkey2.setTexture(monkeyTexture2);
    monkey2.setOrigin(monkey2.getLocalBounds().width / 2, monkey2.getLocalBounds().height / 2);
    monkey2.setPosition(gameWindow.getSize().x/2,150);
    monkey2.setScale(0.5f, 0.5f);
}
void Game::pauseDraw(){
    gameWindow.clear(sf::Color(100,200,255));
    gameWindow.draw(pauseText);
    gameWindow.draw(continueRectPause);
    gameWindow.draw(pauseContinue);
    gameWindow.draw(restartButtonPause);
    gameWindow.draw(pauseRestart);
    gameWindow.draw(saveRect);
    gameWindow.draw(pauseSaveGame);
    gameWindow.draw(menuButtonPause);
    gameWindow.draw(pauseMenu);
    gameWindow.draw(monkey2);
}
void Game::settingsCreation() {
    UIComponents::textCreating(speedSettingsText,"Speed:",120,sf::Color::Green,sf::Color::Blue,4,gameWindow.getSize().x/2,gameWindow.getSize().y/2+200);
    UIComponents::textCreating(settingsTextSettings,"Settings",200,sf::Color::Green,sf::Color::Blue,10,gameWindow.getSize().x/2,gameWindow.getSize().y/2-400);
    UIComponents::textCreating(fontSettingsText,"Fonts:",120,sf::Color::Green,sf::Color::Blue,4,gameWindow.getSize().x/2,gameWindow.getSize().y/2-200);
    UIComponents::textCreating(font1,"F1",80,sf::Color::Blue,sf::Color::Green,4,gameWindow.getSize().x/2-100,gameWindow.getSize().y/2-50);
    UIComponents::textCreating(font2,"F2",80,sf::Color::Blue,sf::Color::Green,4,gameWindow.getSize().x/2,gameWindow.getSize().y/2-50);
    UIComponents::textCreating(font3,"F3",80,sf::Color::Blue,sf::Color::Green,4,gameWindow.getSize().x/2+100,gameWindow.getSize().y/2-50);
    UIComponents::rectCreating(fontChangeButton,gameWindow.getSize().x/2,gameWindow.getSize().y/2+100,80,50,sf::Color::Black,sf::Color::Black,4);
    UIComponents::textCreating(fontChangeText,"->",80,sf::Color::Green,sf::Color::Blue,4,gameWindow.getSize().x/2-10,gameWindow.getSize().y/2+70);

    UIComponents::textCreating(speedIncrease,"+",80,sf::Color::Blue,sf::Color::Green,4,gameWindow.getSize().x/2-100,gameWindow.getSize().y/2+350);
    UIComponents::textCreating(speedDecrease,"-",80,sf::Color::Blue,sf::Color::Green,4,gameWindow.getSize().x/2+100,gameWindow.getSize().y/2+325);
    UIComponents::rectCreating(increaseButton,gameWindow.getSize().x/2-80,gameWindow.getSize().y/2+380,50,50,sf::Color::Black,sf::Color::Black,4);
    UIComponents::rectCreating(decreaseButton,gameWindow.getSize().x/2+110,gameWindow.getSize().y/2+380,50,50,sf::Color::Black,sf::Color::Black,6);
    UIComponents::rectCreating(wellDoneButton,gameWindow.getSize().x/2,gameWindow.getSize().y/2+600,80,80,sf::Color::Black,sf::Color::Black,0);
    UIComponents::textCreating(wellDoneText,"OK",80,sf::Color::Green,sf::Color::Blue,4,gameWindow.getSize().x/2,gameWindow.getSize().y/2+580);
}
void Game::settingsDraw() {
    gameWindow.clear(sf::Color::Black);
    gameWindow.draw(settingsTextSettings);
    gameWindow.draw(fontSettingsText);

    sf::Color highlightColor = sf::Color::Magenta;
    sf::Color normalColor = sf::Color::Blue;
    if (settings.currentFontIndex == 0) {
        font1.setFillColor(highlightColor);
        font2.setFillColor(normalColor);
        font3.setFillColor(normalColor);
    } else if (settings.currentFontIndex == 1) {
        font1.setFillColor(normalColor);
        font2.setFillColor(highlightColor);
        font3.setFillColor(normalColor);
    } else if (settings.currentFontIndex == 2) {
        font1.setFillColor(normalColor);
        font2.setFillColor(normalColor);
        font3.setFillColor(highlightColor);
    }
    gameWindow.draw(font1);
    gameWindow.draw(font2);
    gameWindow.draw(font3);
    gameWindow.draw(fontChangeButton);
    gameWindow.draw(fontChangeText);
    gameWindow.draw(speedSettingsText);
    gameWindow.draw(increaseButton);
    gameWindow.draw(speedIncrease);
    gameWindow.draw(decreaseButton);
    gameWindow.draw(speedDecrease);
    gameWindow.draw(wellDoneButton);
    gameWindow.draw(wellDoneText);
    gameWindow.draw(speedDisplay);
}
void Game::highScoresCreation(){
    UIComponents::textCreating(highScoresLogo,"High Scores",200,sf::Color::Yellow,sf::Color::Red,15,gameWindow.getSize().x/2,gameWindow.getSize().y/2-470);
    UIComponents::rectCreating(exitHighScoresRect,200,gameWindow.getSize().y/2-470,100,100,sf::Color::Black,sf::Color::Black,4);
    UIComponents::textCreating(exitHighScoresText,"back",50,sf::Color::Red,sf::Color::Yellow,4,200,gameWindow.getSize().y/2-490);
    UIComponents::textCreating(playerNameHS,"Player:",100,sf::Color::Yellow,sf::Color::Red,7,gameWindow.getSize().x/2-400,gameWindow.getSize().y/2-200);
    UIComponents::textCreating(scoreHS,"Score:",100,sf::Color::Yellow,sf::Color::Red,7,gameWindow.getSize().x/2+400,gameWindow.getSize().y/2-200);
}
void Game::drawHighScores(){
    gameWindow.clear(sf::Color::Black);
    gameWindow.draw(highScoresLogo);
    gameWindow.draw(exitHighScoresRect);
    gameWindow.draw(exitHighScoresText);
    gameWindow.draw(playerNameHS);
    gameWindow.draw(scoreHS);
    int yOffset = 200;
    for (const auto& entry:leaderboard) {
        sf::Text nameText;
        UIComponents::textCreating(nameText,entry.playerName,80,sf::Color::Blue,sf::Color::Yellow,6,gameWindow.getSize().x/2-400,gameWindow.getSize().y/2-200+yOffset);
        sf::Text scoreText;
        UIComponents::textCreating(scoreText,std::to_string(entry.score),80,sf::Color::Blue,sf::Color::Yellow,6,gameWindow.getSize().x/2+400,gameWindow.getSize().y/2-200+yOffset);

        gameWindow.draw(nameText);
        gameWindow.draw(scoreText);
        yOffset +=100;
    }
}
void Game::drawNameBox(){
    gameWindow.clear(sf::Color(255,182,193));
    gameWindow.draw(nameEntryBox);
    gameWindow.draw(nameTextLogo);
    sf::FloatRect textRect = playerNameDisplay.getLocalBounds();
    playerNameDisplay.setOrigin(textRect.width / 2, textRect.height / 2);
    playerNameDisplay.setPosition(gameWindow.getSize().x / 2-50, gameWindow.getSize().y / 2 + 50);
    gameWindow.draw(playerNameDisplay);
}
void Game::drawGame(){
    gameWindow.clear(sf::Color(150,150,150));
    gameWindow.draw(topLine);
    gameWindow.draw(scoreDisplay);
    gameWindow.draw(timerDisplay);
    gameWindow.draw(trialsDisplay);
    gameWindow.draw(inputBox);
    gameWindow.draw(inputDisplay);
    gameWindow.draw(pauseRect);
    gameWindow.draw(pause);
}