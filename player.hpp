#include <SFML/Graphics.hpp>
#include <iostream>

#include "missiles.hpp"

using namespace std;
using namespace sf;

class Player
{
    Texture texture;
    Sprite sprite;
    int framesCount;
    float maxSpeed, xSpeedDivider, ySpeedDivider;

    RenderWindow &window;
    int windowWidth, windowHeight;

    int width, height, currentFrame = 0;
    float x, y, xSpeed = 0, ySpeed = 0;

    bool isMoving[4];
    Clock movingClock[4];
    int gear[4] = {1, 1, 1, 1};
    enum
    {
        LEFT, RIGHT, UP, DOWN
    };
    Image bulletImage;
    Clock shootingClock;

public:

    Player(Image &image, int FramesCount, float MaxSpeed, RenderWindow &Window)
    : framesCount(FramesCount), maxSpeed(MaxSpeed), window(Window)
    {
        xSpeedDivider = maxSpeed / (framesCount - 1);
        ySpeedDivider = maxSpeed / ((framesCount - 1) * 2);
        windowWidth = window.getSize().x;
        windowHeight = window.getSize().y;
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        width = texture.getSize().x / framesCount;
        height = texture.getSize().y;
        sprite.setOrigin(width / 2, height / 2);
        sprite.setTextureRect(IntRect(0, 0, width, height));
        x = windowWidth / 2;
        y = window.getSize().y - window.getSize().y / 10;
        sprite.setPosition(x, y);
        bulletImage.loadFromFile("images/defaultMissile.png");
    }

    void update(float &mainTime, list <Missile*> &missiles)
    {
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            if (gear[LEFT] < framesCount && movingClock[LEFT].getElapsedTime().asMilliseconds() >= 100)
            {
                gear[LEFT]++;
                movingClock[LEFT].restart();
            }
        }
        else if (gear[LEFT] > 1 && movingClock[LEFT].getElapsedTime().asMilliseconds() >= 100)
        {
            gear[LEFT]--;
            movingClock[LEFT].restart();
        }

        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            if (gear[RIGHT] < framesCount && movingClock[RIGHT].getElapsedTime().asMilliseconds() >= 100)
            {
                gear[RIGHT]++;
                movingClock[RIGHT].restart();
            }
        }
        else if (gear[RIGHT] > 1 && movingClock[RIGHT].getElapsedTime().asMilliseconds() >= 100)
        {
            gear[RIGHT]--;
            movingClock[RIGHT].restart();
        }

        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            if (gear[UP] < 12 && movingClock[UP].getElapsedTime().asMilliseconds() >= 100)
            {
                gear[UP]++;
                movingClock[UP].restart();
            }
        }
        else if (gear[UP] > 1 && movingClock[UP].getElapsedTime().asMilliseconds() >= 100)
        {
            gear[UP]--;
            movingClock[UP].restart();
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            if (gear[DOWN] < 12 && movingClock[DOWN].getElapsedTime().asMilliseconds() >= 100)
            {
                gear[DOWN]++;
                movingClock[DOWN].restart();
            }
        }
        else if (gear[DOWN] > 1 && movingClock[DOWN].getElapsedTime().asMilliseconds() >= 100)
        {
            gear[DOWN]--;
            movingClock[DOWN].restart();
        }

        xSpeed = (xSpeedDivider * (gear[1] - gear[0])) * mainTime;
        ySpeed = (ySpeedDivider * (gear[3] - gear[2])) * mainTime;

        x += xSpeed; y += ySpeed;
        if (x < 0 || x > windowWidth) x -= xSpeed;
        if (y > windowHeight || y < windowHeight / 2) y -= ySpeed;

        sprite.setPosition(x, y);

        currentFrame = gear[0] - gear[1];
        if (currentFrame < 0) sprite.setTextureRect(IntRect(-currentFrame * width, 0, -width, height));
        else sprite.setTextureRect(IntRect(currentFrame * width, 0, width, height));

        window.draw(sprite);

        if (Keyboard::isKeyPressed(Keyboard::Space) && shootingClock.getElapsedTime().asMilliseconds() > 300)
        {
            missiles.push_back(new Missile(bulletImage, 1, 10, x, y - 55, window));
            cout << missiles.size() << endl;
            shootingClock.restart();
        }
    }

};
