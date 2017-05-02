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

    Clock mainClock, FPSClock;
    float mainTime, FPSTimer;

    Font font;
    font.loadFromFile("font.ttf");

    Image starsImage;
    starsImage.loadFromFile("images/stars.png");
    Background background(starsImage, 20, 0.6, 10, 15, window);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) window.close();
        }

        mainTime = mainClock.getElapsedTime().asMicroseconds();
        mainTime /= 800;
        mainClock.restart();

        window.clear();
        background.update(mainTime);
        window.display();

    }

    return 0;
}
