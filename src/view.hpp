#include "map.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;
#ifndef VIEW_HPP
#define VIEW_HPP
View view, viewP2;

void setPlayerCoordinateForView(float x, float y)
{
    float tempX = x, tempY = y;
    int halfW = 480, halfH = 540, tileSize = 64;

    if (x < halfW)
        tempX = halfW;
    if (x > tileSize * WIDTH_MAP - halfW)
        tempX = tileSize * WIDTH_MAP - halfW;
    if (y < halfH)
        tempY = halfH;
    if (y > tileSize * HEIGHT_MAP - halfH)
        tempY = tileSize * HEIGHT_MAP - halfH;

    view.setCenter(tempX, tempY);
}
void setPlayer2CoordinateForView(float x, float y)
{
    float tempX = x, tempY = y;
    int halfW = 480, halfH = 540, tileSize = 64;

    if (x < halfW)
        tempX = halfW;
    if (x > tileSize * WIDTH_MAP - halfW)
        tempX = tileSize * WIDTH_MAP - halfW;
    if (y < halfH)
        tempY = halfH;
    if (y > tileSize * HEIGHT_MAP - halfH)
        tempY = tileSize * HEIGHT_MAP - halfH;

    viewP2.setCenter(tempX, tempY);
}
void changeView()
{
    if (Keyboard::isKeyPressed(Keyboard::E)) {
        view.setSize(960, 1080);
        view.setViewport(FloatRect(0, 0, 0.5f, 1));
        viewP2.setSize(960, 1080);
        viewP2.setViewport(FloatRect(0.5f, 0, 0.5f, 1));
    }
    if (Keyboard::isKeyPressed(Keyboard::Q)) {
        view.setSize(1920, 1080);
        view.setViewport(FloatRect(0, 0, 1, 1));
        viewP2.setSize(0, 0);
    }
}
#endif
