#include <SFML/Graphics.hpp>
#include <vector>

#include "functions.hpp"

using namespace std;
using namespace sf;

class Background
{

    Texture texture;
    int starsCount, lowerScaleLimit, upperScaleLimit, windowWidth, windowHeight;
    float speedMult;

    RenderWindow &window;

public:

    //Текстура со звёздами в один ряд, количество вариантов звёзд на изображении, количество звёзд на фоне, мультипликатор скорости, границы для рандома размера звезды, окно
    //Решил отказаться от выбора рандомной текстуры звезды, оставил только четырехконечную, так звездное пространство выглядит более реалистично и чуть менее нагружает систему и код (позволило избавиться от трёх(5) переменных)
    //Чем выше число границы размера, тем меньше звезда (формула scale = 1 / randomLimit)
    Background(Image &image, int StarsCount, float SpeedMult, int LowerScaleLimit, int UpperScaleLimit, RenderWindow &Window)
    : starsCount(StarsCount), lowerScaleLimit(LowerScaleLimit), upperScaleLimit(UpperScaleLimit), speedMult(SpeedMult), window(Window)
    {
        texture.loadFromImage(image);
        windowWidth = window.getSize().x;
        windowHeight = window.getSize().y;
        for (int i = 0; i < starsCount; i++)
        {
            Star star(texture);
            stars.push_back(star);
            stars[i].createNew(speedMult, lowerScaleLimit, upperScaleLimit, windowHeight, windowWidth, true);
        }
    }

    void update(float &mainTime)
    {
        for (int i = 0; i < starsCount; i++)
        {
            stars[i].sprite.move(0, stars[i].speed * mainTime);
            stars[i].y += stars[i].speed * mainTime;
            if (stars[i].y > windowHeight + 100) stars[i].createNew(speedMult, lowerScaleLimit, upperScaleLimit, windowWidth);
            window.draw(stars[i].sprite);
        }
    }

private:

    class Star
    {
    public:

        Sprite sprite;
        float scale, speed, y;

        Star(Texture &texture)
        {
            sprite.setTexture(texture);
            sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y);
        }

        void createNew(float &speedMult, int &lowerScaleLimit, int &upperScaleLimit, int &windowWidth, int windowHeight = 0, bool firstTime = false)
        {
            scale = 1 / (float)randomNumber(lowerScaleLimit, upperScaleLimit);
            sprite.setScale(scale, scale);
            speed = scale * speedMult;
            if (firstTime) y = randomNumber(0, windowHeight);
            else y = 0;
            sprite.setPosition(randomNumber(0, windowWidth), y);
        }

    };

    vector <Star> stars;
};
