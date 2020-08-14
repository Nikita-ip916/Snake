#include "map.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;
#ifndef VIEW_HPP
#define VIEW_HPP
View view, viewP2;

void setPlayerCoordinateForView(float x, float y, float speed)
{
    float tempX = x, tempY = y;
    int halfW = 960, halfH = 540, tileSize = 64;

    if (x < halfW)
        tempX = halfW;
    if (x > tileSize * WIDTH_MAP - halfW)
        tempX = tileSize * WIDTH_MAP - halfW;
    if (y < halfH)
        tempY = halfH;
    if (y > tileSize * HEIGHT_MAP - halfH)
        tempY = tileSize * HEIGHT_MAP - halfH;

    view.setCenter(
            view.getCenter().x + (tempX - view.getCenter().x) / (48 / speed),
            view.getCenter().y + (tempY - view.getCenter().y) / (27 / speed));
}
void setPlayer1CoordinateForView(float x, float y, float speed)
{
    float tempX = x, tempY = y;
    int halfW = 474, halfH = 540, tileSize = 64;

    if (x < halfW)
        tempX = halfW;
    if (x > tileSize * WIDTH_MAP - halfW)
        tempX = tileSize * WIDTH_MAP - halfW;
    if (y < halfH)
        tempY = halfH;
    if (y > tileSize * HEIGHT_MAP - halfH)
        tempY = tileSize * HEIGHT_MAP - halfH;

    view.setCenter(
            view.getCenter().x + (tempX - view.getCenter().x) / (23.7 / speed),
            view.getCenter().y + (tempY - view.getCenter().y) / (27 / speed));
}
void setPlayer2CoordinateForView(float x, float y, float speed)
{
    float tempX = x, tempY = y;
    int halfW = 474, halfH = 540, tileSize = 64;

    if (x < halfW)
        tempX = halfW;
    if (x > tileSize * WIDTH_MAP - halfW)
        tempX = tileSize * WIDTH_MAP - halfW;
    if (y < halfH)
        tempY = halfH;
    if (y > tileSize * HEIGHT_MAP - halfH)
        tempY = tileSize * HEIGHT_MAP - halfH;

    viewP2.setCenter(
            viewP2.getCenter().x
                    + (tempX - viewP2.getCenter().x) / (23.7 / speed),
            viewP2.getCenter().y
                    + (tempY - viewP2.getCenter().y) / (27 / speed));
}
void changeView(string plNumber)
{
    if (plNumber == "2") {
        view.setSize(948, 1080);
        view.setViewport(FloatRect(0, 0, 0.49375f, 1));
        view.setCenter(474, 540);
        viewP2.setSize(948, 1080);
        viewP2.setViewport(FloatRect(0.50625f, 0, 0.49375f, 1));
        view.setCenter(474, 540);
    }
    if (plNumber == "1") {
        view.setSize(1920, 1080);
        view.setViewport(FloatRect(0, 0, 1, 1));
        view.setCenter(960, 540);
        viewP2.setSize(0, 0);
    }
}
#endif
