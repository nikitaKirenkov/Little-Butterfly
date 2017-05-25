#include <SFML/Graphics.hpp>

#include "functions.hpp"
#include "missiles.hpp"

using namespace sf;

class Enemy
{
    Sprite bodySprite, cannonSprite;
    Texture bodyTexture, cannonTexture;
    int type, health, damage, x, width, height;
    float angle = 0, y, scale = 1;
    RenderWindow &window;
    Clock hitClock;
public:

    Enemy(Image &bodyImage, Image &cannonImage, int Type, int X, RenderWindow &Window) : type(Type), x(X), window(Window)
    {
        switch (type)
        {
            case 0: health = 100; break;
        }
        bodyTexture.loadFromImage(bodyImage);
        bodySprite.setTexture(bodyTexture);
        cannonTexture.loadFromImage(cannonImage);
        cannonSprite.setTexture(cannonTexture);
        width = bodyTexture.getSize().x;
        height = bodyTexture.getSize().y;
        bodySprite.setOrigin(width / 2, height / 2);
        cannonSprite.setOrigin(width / 2, height / 2);
        width *= scale;
        height *= scale;
        y = -height;
        bodySprite.setPosition(x, y);
        cannonSprite.setPosition(x, y);
    }

    void update(float &mainTime, list <Missile*> &missiles, list <Missile*>::iterator &it)
    {
        //cout << bodyTexture.getSize().y << " " << y << endl;
        if (y < height)
        {
            bodySprite.move(0, mainTime * 0.2); cannonSprite.move(0, mainTime * 0.2);
            y += mainTime * 0.2;
        }
        if (Keyboard::isKeyPressed(Keyboard::Num1))
        angle += mainTime * 0.05;
        if (Keyboard::isKeyPressed(Keyboard::Num2))
            angle -= mainTime * 0.05;
        cannonSprite.setRotation(angle);
        for (it = missiles.begin(); it != missiles.end(); it++)
        {
            if (FloatRect(x - width / 2, y - width / 2, width, height).intersects((*it)->getRect()))
            {
                hitClock.restart();
                health -= (*it)->damage;
                (*it)->isAlive = false;
            }
        }
        if (health <= 0) isAlive = false;
        if (hitClock.getElapsedTime().asMilliseconds() > 100)
        {
            window.draw(bodySprite);
            window.draw(cannonSprite);
        }
    }

    bool isAlive = true;
};
