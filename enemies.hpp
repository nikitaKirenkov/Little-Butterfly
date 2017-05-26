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
    int type, health, damage, width, height;
    float x, y, angle = 0, playerAngle, scale = 1, windowWidth;
    RenderWindow &window;
    Clock hitClock, shootingClock, angleClock;
    Image missileImage;
    double missileSpeed = 0.2;
    bool movingRight;
public:

    Enemy(Image &bodyImage, Image &cannonImage, int Type, int X, RenderWindow &Window) : type(Type), x(X), window(Window)
    {
        windowWidth = window.getSize().x;
        movingRight = randomNumber(0, 1);
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

    void update(float &mainTime, float lowerMovingLimit, float upperMovingLimit, Player &player, list <Missile*> &enemyMissiles, list <Missile*> &playerMissiles, list <Missile*>::iterator &playerMissilesIt)
    {
        playerAngle = atan(((windowWidth - x) - (windowWidth - player.x)) / (y - player.y)) / PI * 180;
        if (angle < playerAngle - 2) angle += mainTime * 0.01;
        else if (angle > playerAngle + 2) angle -= mainTime * 0.01;
        cannonSprite.setRotation(angle);
        if (y < height)
        {
            bodySprite.move(0, mainTime * 0.2); cannonSprite.move(0, mainTime * 0.2);
            y += mainTime * 0.2;
        }
        else
        {
            if (shootingClock.getElapsedTime().asMilliseconds() > 1500)
            {
                enemyMissiles.push_back(new Missile(missileImage, 0.4, missileSpeed * cos((90 + angle) * PI / 180), missileSpeed * sin((90 - angle) * PI / 180), 10,
                x - width / 2 * cos((-angle + 90) * PI / 180), y + height / 2 * sin((-angle + 90) * PI / 180), window));
                shootingClock.restart();
            }
            if (movingRight && x < upperMovingLimit - width / 2 - 10)
            {
                bodySprite.move(mainTime * 0.1, 0); cannonSprite.move(mainTime * 0.1, 0); x += mainTime * 0.1;
            }
            else if (x > lowerMovingLimit + width / 2 + 10)
            {
                bodySprite.move(-mainTime * 0.1, 0); cannonSprite.move(-mainTime * 0.1, 0); x -= mainTime * 0.1;
                movingRight = false;
            }
            else movingRight = true;
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
        if (health <= 0)
        {
            isAlive = false;
            player.score += 100;
            player.ammo += randomNumber(10, 20);
            player.health += randomNumber(0, 5);
        }
        if (hitClock.getElapsedTime().asMilliseconds() > 100)
        {
            window.draw(bodySprite);
            window.draw(cannonSprite);
        }
    }

    bool isAlive = true;
};
