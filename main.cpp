#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

#include "functions.hpp"
#include "background.hpp"
#include "player.hpp"

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
    bool gameIsPaused = false;

    Font font;
    font.loadFromFile("font.ttf");

    Text FPSCounter;
    FPSCounter.setFont(font);
    FPSCounter.setFillColor(Color::Red);
    FPSCounter.setPosition(10, 10);

    Text pauseText;
    pauseText.setFont(font);
    pauseText.setFillColor(Color::Red);
    pauseText.setString("PAUSE");
    pauseText.setPosition(windowWidth / 2 - pauseText.getCharacterSize() * 2, windowHeight / 2);

    Image starsImage;
    starsImage.loadFromFile("images/stars.png");
    Background background(starsImage, 60, 2.6, 10, 15, window);

    Image playerImage;
    playerImage.loadFromFile("images/player.png");
    Player player(playerImage, 7, 0.6, window);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::P)
            {
                gameIsPaused = !gameIsPaused;
                window.draw(pauseText);
                window.display();
            }
        }

        mainTime = mainClock.getElapsedTime().asMicroseconds();
        mainTime /= 800;
        mainClock.restart();

        if (!gameIsPaused)
        {
            window.clear();
            background.update(mainTime);
            player.update(mainTime);

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
            window.display();
        }
    }

    return 0;
}
