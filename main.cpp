#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <list>

#include "functions.hpp"
#include "background.hpp"
#include "player.hpp"
#include "missiles.hpp"
#include "enemies.hpp"

using namespace std;
using namespace sf;

int main()
{
    srand(time(NULL));

    String windowTitle = "Game";
    unsigned int windowWidth, windowHeight;
    int screenMode = 4;
    bool fullScreenEnabled, verticalSyncEnabled, FPSCounterEnabled;

    //загрузка настроек из конфига
    string *tmp = new string;
    ifstream fin("config.ini");
    fin >> *tmp >> *tmp >> windowWidth;
    fin >> *tmp >> *tmp >> windowHeight;
    fin >> *tmp >> *tmp >> fullScreenEnabled;
    fin >> *tmp >> *tmp >> verticalSyncEnabled;
    fin >> *tmp >> *tmp >> FPSCounterEnabled;
    fin.close();
    delete tmp;
    //
    if (fullScreenEnabled) screenMode = 8;
    RenderWindow window(VideoMode(windowWidth, windowHeight), windowTitle, screenMode);
    window.setVerticalSyncEnabled(verticalSyncEnabled);
    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);

    Clock mainClock, FPSClock;
    float mainTime;
    int FPS = 0;
    bool gameIsPaused = false, gameIsOver = false;

    Font font;
    font.loadFromFile("font.ttf");

    Text FPSCounter;
    FPSCounter.setFont(font);
    FPSCounter.setFillColor(Color::Red);
    FPSCounter.setString("FPS: 0");
    FPSCounter.setPosition(10, 10);

    Text pauseText;
    pauseText.setFont(font);
    pauseText.setFillColor(Color::Red);
    pauseText.setString("PAUSE");
    pauseText.setPosition(windowWidth / 2 - pauseText.getCharacterSize() * 2, windowHeight / 2);

    Text playerStatsCount;
    playerStatsCount.setFont(font);
    playerStatsCount.setFillColor(Color::Red);
    playerStatsCount.setString("AMMO: 100 HEALTH: 100 SCORE: 0");
    playerStatsCount.setCharacterSize(24);
    playerStatsCount.setPosition(windowWidth / 2 - playerStatsCount.getCharacterSize() * 10, windowHeight - windowHeight / 20);

    Image starsImage;
    starsImage.loadFromFile("images/stars.png");
    Background background(starsImage, 60, 2.6, 10, 15, window);

    Image playerImage;
    playerImage.loadFromFile("images/player.png");
    Player player(playerImage, 7, 0.6, window);

    list <Missile*> playerMissiles;
    list <Missile*>::iterator playerMissilesIt;

    list <Missile*> enemyMissiles;
    list <Missile*>::iterator enemyMissilesIt;

    list <Enemy*> enemies;
    list <Enemy*>::iterator enemiesIt;

    Image enemyBodyImage;
    enemyBodyImage.loadFromFile("images/enemyBody.png");
    Image enemyCannonImage;
    enemyCannonImage.loadFromFile("images/enemyCannon.png");

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::P && !gameIsOver)
            {
                gameIsPaused = !gameIsPaused;
                window.draw(pauseText);
                window.display();
            }
            if (gameIsOver && event.type == Event::KeyPressed && event.key.code == Keyboard::Return)
            {
                player.reset();
                enemies.clear();
                playerMissiles.clear();
                enemyMissiles.clear();
                gameIsOver = false;
            }
        }

        mainTime = mainClock.getElapsedTime().asMicroseconds();
        mainTime /= 800;
        mainClock.restart();
        if (!gameIsPaused && !gameIsOver)
        {
            window.clear();
            background.update(mainTime);
            if (FPSCounterEnabled)
            {
                FPS++;
                if (FPSClock.getElapsedTime().asMilliseconds() >= 1000)
                {
                    FPSCounter.setString("FPS: " + intToString(FPS));
                    FPS = 0;
                    FPSClock.restart();
                }
                window.draw(FPSCounter);
            }
            window.draw(playerStatsCount);
            if (enemies.size() == 0)
            {
                int enemiesCount = randomNumber(1, 3);
                for (int i = 1; i <= enemiesCount; i++)
                {
                    enemies.push_back(new Enemy(enemyBodyImage, enemyCannonImage, 0, i * windowWidth / enemiesCount - windowWidth / enemiesCount / 2, window));
                }
            }
            int i = 1;
            for (enemiesIt = enemies.begin(); enemiesIt != enemies.end();)
            {
                (*enemiesIt)->update(mainTime, windowWidth / enemies.size() * (i - 1), windowWidth / enemies.size() * i, player, enemyMissiles, playerMissiles, playerMissilesIt);
                if (!(*enemiesIt)->isAlive) enemiesIt = enemies.erase(enemiesIt);
                else { enemiesIt++; i++; }
            }
            for (playerMissilesIt = playerMissiles.begin(); playerMissilesIt != playerMissiles.end();)
            {
                (*playerMissilesIt)->update(mainTime);
                if (!(*playerMissilesIt)->isAlive) playerMissilesIt = playerMissiles.erase(playerMissilesIt);
                else playerMissilesIt++;
            }
            for (enemyMissilesIt = enemyMissiles.begin(); enemyMissilesIt != enemyMissiles.end();)
            {
                (*enemyMissilesIt)->update(mainTime);
                if (!(*enemyMissilesIt)->isAlive) enemyMissilesIt = enemyMissiles.erase(enemyMissilesIt);
                else enemyMissilesIt++;
            }
            player.update(mainTime, playerMissiles, enemyMissiles, enemyMissilesIt);
            playerStatsCount.setString("AMMO: " + intToString(player.ammo) + " HEALTH: " + intToString(player.health) + " SCORE: " + intToString(player.score));
            if (!player.isAlive || (player.ammo == 0 && playerMissiles.size() == 0))
            {
                gameIsOver = true;
                pauseText.setString("        GAME OVER\n press enter to restart");
                window.draw(pauseText);
            }

            window.display();
        }
    }

    return 0;
}
