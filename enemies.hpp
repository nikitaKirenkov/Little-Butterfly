#include <SFML/Graphics.hpp>
#include <math.h>

#include "functions.hpp"
#include "missiles.hpp"
#include "player.hpp"

using namespace sf;

class Enemy
{
    Sprite bodySprite, cannonSprite;
    Texture bodyTexture, cannonTexture;
    int type, health, damage, x, width, height;
    float y, angle = 0, scale = 1;
    RenderWindow &window;
    Clock hitClock, shootingClock, angleClock;
    Image missileImage;
    double missileSpeed = 0.2;
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
        missileImage.loadFromFile("images/enemyMissile.png");
    }

    void update(float &mainTime, Player &player, list <Missile*> &enemyMissiles, list <Missile*> &playerMissiles, list <Missile*>::iterator &playerMissilesIt)
    {
        if (y < height)
        {
            bodySprite.move(0, mainTime * 0.2); cannonSprite.move(0, mainTime * 0.2);
            y += mainTime * 0.2;
        }
        if (Keyboard::isKeyPressed(Keyboard::Num1))
        angle += mainTime * 0.1;
        if (Keyboard::isKeyPressed(Keyboard::Num2))
            angle -= mainTime * 0.1;

        cannonSprite.setRotation(angle);
        if (shootingClock.getElapsedTime().asMilliseconds() > 400)
        {
            enemyMissiles.push_back(new Missile(missileImage, 0.4, missileSpeed * cos((90 + angle) * PI / 180), missileSpeed * sin((90 - angle) * PI / 180), 10,
            x - width / 2 * cos((-angle + 90) * PI / 180), y + height / 2 * sin((-angle + 90) * PI / 180), window));
            cout << angle << endl;
            shootingClock.restart();
        }
        for (playerMissilesIt = playerMissiles.begin(); playerMissilesIt != playerMissiles.end(); playerMissilesIt++)
        {
            if (FloatRect(x - width / 2, y - width / 2, width, height).intersects((*playerMissilesIt)->getRect()))
            {
                hitClock.restart();
                health -= (*playerMissilesIt)->damage;
                (*playerMissilesIt)->isAlive = false;
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
