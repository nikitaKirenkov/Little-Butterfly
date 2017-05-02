#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <time.h>

using namespace std;
using namespace sf;

int randomNumber(int from, int to)
{
    return from + rand() % (to - from + 1);
}

int windowWidth = 1280, windowHeight = 720;
RenderWindow window(VideoMode(windowWidth, windowHeight), "Game");

float mainTime;

Texture starTexture;
class Star
{

    Sprite sprite;

    float x, y, scale, speed, speedMult = 1.6;

    void createNewStar(float Y = -1000)
    {
        switch (randomNumber(1, 5))
        {
            case 1: sprite.setColor(Color::White); break;
            case 2: sprite.setColor(Color::Yellow); break;
            case 3: sprite.setColor(Color::Red); break;
            case 4: sprite.setColor(Color::Green); break;
            case 5: sprite.setColor(Color::Blue); break;
        }
        sprite.setTextureRect(IntRect(100 * randomNumber(0, 2), 0, 100, 100));
        scale = 1 / (float)randomNumber(7, 15);
        sprite.setScale(scale, scale);
        speed = scale * speedMult;
        x = randomNumber(-50, windowWidth - 50);
        if (Y == -1000)  y = randomNumber(-50, windowHeight + 50);
        else y = Y;
        sprite.setPosition(x, y);
    }

public:

    Star()
    {
        starTexture.loadFromFile("images/stars.png");
        sprite.setTexture(starTexture);
        createNewStar();
        window.draw(sprite);
    }

    void update()
    {
        y += speed;
        sprite.setPosition(x, y);
        if (y > windowHeight)
        {
            createNewStar(-50);
        }
        window.draw(sprite);
    }

    void changeSpeedMult(float on = 0.01)
    {
        speedMult += on;
        speed = scale * speedMult;
    }
};

const int starsNumber = 20;
Star stars[starsNumber];

void createStars()
{
    for (int i = 0; i > starsNumber; i++)
    {
        Star star;
        stars[i] = star;
    }
}

void updateStars()
{
    for (int i = 0; i < starsNumber; i++)
    {
        stars[i].update();
    }
}

class Player
{
    Texture texture;
    Sprite sprite;

    float maxSpeed = 0.6, currentSpeed, xPosition, xValue;

    bool isMovingRight, isMovingLeft, arrowUpIsPressed, arrowDownIsPressed;
    Clock movingRightClock, movingLeftClock;
    float movingRightTime, movingLeftTime;

    float movingStagesNumber = 6, turnTime = 100;
    int currentFrame = 0, movingLeftStage = 0, movingRightStage = 0, gear = 5;

    void checkKeyboard()
    {
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            if (!isMovingLeft)
            {
                isMovingLeft = true;
                movingLeftClock.restart();
            }
        }
        else if (isMovingLeft)
        {
            isMovingLeft = false;
        }

        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            if (!isMovingRight)
            {
                isMovingRight = true;
                movingLeftClock.restart();
            }
        }
        else if (isMovingRight)
        {
            isMovingRight = false;
        }

        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            if (!arrowUpIsPressed)
            {
                accelerate();
                arrowUpIsPressed = true;
            }
        }
        else
        {
            arrowUpIsPressed = false;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            if (!arrowDownIsPressed)
            {
                decelerate();
                arrowDownIsPressed = true;
            }
        }
        else
        {
            arrowDownIsPressed = false;
        }
    }

    void accelerate()
    {
        if (gear < 10)
        {
            turnTime -= 10;
            for (int i = 0; i < starsNumber; i++)
            {
                stars[i].changeSpeedMult(0.3);
            }
            gear++;
        }
    }

    void decelerate()
    {
        if (gear > 1)
        {
            turnTime += 10;
            for (int i = 0; i < starsNumber; i++)
            {
                stars[i].changeSpeedMult(-0.3);
            }
            gear--;
        }
    }

public:

    Player(int xPos, int yPos)
    {

        texture.loadFromFile("images/player.png");
        sprite.setTexture(texture);
        sprite.setPosition(xPos, yPos);
        xPosition = xPos;
    }

    void update()
    {
        checkKeyboard();
        movingLeftTime = movingLeftClock.getElapsedTime().asMilliseconds();
        movingRightTime = movingRightClock.getElapsedTime().asMilliseconds();

        if (movingLeftTime >= turnTime)
        {
            if (isMovingLeft && movingLeftStage < movingStagesNumber)
            {
                movingLeftStage++;
                currentSpeed -= maxSpeed / movingStagesNumber;
            }
            else if (!isMovingLeft && movingLeftStage > 0)
            {
                movingLeftStage--;
                currentSpeed += maxSpeed / movingStagesNumber;
            }

            movingLeftClock.restart();
        }


        if (movingRightTime >= turnTime)
        {
            if (isMovingRight && movingRightStage < movingStagesNumber)
            {
                movingRightStage++;
                currentSpeed += maxSpeed / movingStagesNumber;
            }
            else if (!isMovingRight && movingRightStage > 0)
            {
                movingRightStage--;
                currentSpeed -= maxSpeed / movingStagesNumber;
            }

            movingRightClock.restart();
        }

        xValue = currentSpeed * mainTime;
        if (xPosition + xValue > 0 && xPosition + xValue < windowWidth - 120)
        {
            sprite.move(xValue, 0);
            xPosition += xValue;
        }

        currentFrame = currentSpeed / (maxSpeed / movingStagesNumber);
        if (currentFrame < 0)
        {
            sprite.setTextureRect(IntRect(120 * -currentFrame, 0, 120, 120));
        }
        else
        {
            sprite.setTextureRect(IntRect(120 * currentFrame + 120, 0, -120, 120));
        }
    }

    Sprite getSprite()
    {
        return sprite;
    }
};

int main(void)
{
    srand(time(NULL));

    Clock mainClock;

    Player player(windowWidth / 2 - 50, windowHeight / 3 * 2);

    createStars();
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        mainTime = mainClock.getElapsedTime().asMicroseconds();
        mainClock.restart();
        mainTime /= 800;

        window.clear();

        player.update();
        updateStars();

        window.draw(player.getSprite());
        window.display();
    }

    return 0;
}
