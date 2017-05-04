#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class Player
{
    Texture texture;
    Sprite sprite;
    int framesCount;
    float  animationSpeed, maxSpeed;
    int damage, maxHealth;

    RenderWindow &window;
    int windowWidth;

    Clock frameClock;
    int width, height, currentFrame = 0;
    float x, y, mousePositionX, lastMousePositionX = -11, currentSpeed = 0, windowDivider;

public:

    Player(Image &image, int FramesCount, float AnimationSpeed, float MaxSpeed, int Damage, int MaxHealth, RenderWindow &Window)
    : framesCount(FramesCount), animationSpeed(AnimationSpeed), maxSpeed(MaxSpeed), damage(Damage), maxHealth(MaxHealth), window(Window)
    {
        windowWidth = window.getSize().x;
        windowDivider = (float)windowWidth / ((float)framesCount * 2 - 1);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        width = texture.getSize().x / framesCount;
        height = texture.getSize().y;
        sprite.setOrigin(width / 2, height);
        sprite.setTextureRect(IntRect(0, 0, width, height));
        x = windowWidth / 2;
        y = window.getSize().y - window.getSize().y / 10;
        sprite.setPosition(x, y);
    }

    void update(float &mainTime)
    {
        x = Mouse::getPosition(window).x;
        sprite.setPosition(x, y);
        currentFrame = x / windowDivider;
        if (currentFrame < framesCount) sprite.setTextureRect(IntRect(width * (framesCount - currentFrame - 1), 0, width, height));
        else sprite.setTextureRect(IntRect(width * (currentFrame - framesCount + 2), 0, -width, height));
        window.draw(sprite);
    }

};
