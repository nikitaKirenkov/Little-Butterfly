#ifndef MISSILES_HPP
#define MISSILES_HPP
#include <SFML/Graphics.hpp>

using namespace sf;

class Missile
{
    Texture texture;
    Sprite sprite;
    float speed;
    float x, y, width, height, scale = 0.4;
    RenderWindow &window;

public:
int damage;
    Missile(Image &image, float Speed, int Damage, float X, float Y, RenderWindow &Window) : speed(Speed), x(X), y(Y), window(Window)
    {
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
        sprite.move(0, -speed * mainTime);
        y -= speed * mainTime;
        if (y < 0) isAlive = false;
        window.draw(sprite);
    }

    FloatRect getRect()
    {
        return FloatRect(x - width / 2, y - height / 2, width, height);
    }

    bool isAlive = true;

};

#endif // MISSILES_HPP
