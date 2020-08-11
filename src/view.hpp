#include <SFML/Graphics.hpp>
using namespace sf;
#ifndef VIEW_HPP
#define VIEW_HPP
View view, viewP2;

void setPlayerCoordinateForView(float x, float y)
{
    float tempX = x;
    float tempY = y;

    if (x < 960)
        tempX = 960;
    if (x > 1088)
        tempX = 1088;
    if (y < 540)
        tempY = 540;
    if (y > 1508)
        tempY = 1508;

    view.setCenter(tempX, tempY);
}
void setPlayer2CoordinateForView(float x, float y)
{
    float tempX = x;
    float tempY = y;

    if (x < 960)
        tempX = 960;
    if (x > 1088)
        tempX = 1088;
    if (y < 540)
        tempY = 540;
    if (y > 1508)
        tempY = 1508;

    viewP2.setCenter(tempX, tempY);
}
#endif
