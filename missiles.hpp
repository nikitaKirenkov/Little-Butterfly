#ifndef MISSILES_HPP
#define MISSILES_HPP
#include <SFML/Graphics.hpp>

using namespace sf;

class Missile
{
    Texture texture;
    Sprite sprite;
    float scale, xSpeed, ySpeed;
    float x, y, width, height;
    RenderWindow &window;
    int windowWidth, windowHeight;

public:
int damage;
    Missile(Image &image, float Scale, float XSpeed, float YSpeed, int Damage, float X, float Y, RenderWindow &Window)
    : scale(Scale), xSpeed(XSpeed), ySpeed(YSpeed), x(X), y(Y), window(Window)
    {
        windowWidth = window.getSize().x;
        windowHeight = window.getSize().y;
        damage = Damage;
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        width = texture.getSize().x;
        height = texture.getSize().y;
        sprite.setOrigin(width / 2, height / 2);
        width *= scale; height *= scale;
        sprite.setScale(scale, scale);
        sprite.setPosition(x, y);
    }

    void update(float &mainTime)
    {
        sprite.move(xSpeed * mainTime, ySpeed * mainTime);
        x += xSpeed * mainTime;
        y += ySpeed * mainTime;
        if (y < 0 || y > windowHeight || x < 0 || x > windowWidth) isAlive = false;
        window.draw(sprite);
    }

    FloatRect getRect()
    {
        return FloatRect(x - width / 2, y - height / 2, width, height);
    }

    bool isAlive = true;

};

#endif // MISSILES_HPP
