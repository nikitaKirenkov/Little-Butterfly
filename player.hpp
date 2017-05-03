#include <SFML/Graphics.hpp>

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
    float x, mousePositionX, currentSpeed = 0, frameTimer;

public:

    Player(Image &image, int FramesCount, float AnimationSpeed, float MaxSpeed, int Damage, int MaxHealth, RenderWindow &Window)
    : framesCount(FramesCount), animationSpeed(AnimationSpeed), maxSpeed(MaxSpeed), damage(Damage), maxHealth(MaxHealth), window(Window)
    {
        windowWidth = window.getSize().x;
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        width = texture.getSize().x / framesCount;
        height = texture.getSize().y;
        sprite.setOrigin(width / 2, height);
        sprite.setTextureRect(IntRect(0, 0, width, height));
        x = windowWidth / 2;
        sprite.setPosition(x, window.getSize().y - window.getSize().y / 10);
    }

    void update(float &mainTime)
    {
        mousePositionX = Mouse::getPosition(window).x;
        frameTimer = frameClock.getElapsedTime().asMilliseconds();
        currentSpeed = 0;
        if (mousePositionX < x - 10 && x > 0)
        {
            currentSpeed = -maxSpeed * mainTime;
            if (frameTimer > animationSpeed && currentFrame < framesCount - 1)
            {
                if (currentFrame < 0) currentFrame++;
                currentFrame++;
                frameClock.restart();

            }
        }
        else if (mousePositionX > x + 10 && x < windowWidth)
        {
            currentSpeed = maxSpeed * mainTime;
            if (frameTimer > animationSpeed && currentFrame > -framesCount + 1)
            {
                if (currentFrame > 0) currentFrame--;
                currentFrame--;
                frameClock.restart();
            }
        }
        else if (frameTimer > animationSpeed)
        {
            if (currentFrame > 0) currentFrame--;
            else if (currentFrame < 0) currentFrame++;
            frameClock.restart();
        }

        if (currentFrame < 0) sprite.setTextureRect(IntRect(width * -currentFrame, 0, -width, height));
        else sprite.setTextureRect(IntRect(width * currentFrame, 0, width, height));

        x += currentSpeed;
        sprite.move(currentSpeed, 0);
        window.draw(sprite);
    }

};
