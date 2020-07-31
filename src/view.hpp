#include <SFML/Graphics.hpp>
using namespace sf;
#ifndef VIEW_HPP
#define VIEW_HPP
View view;

void setPlayerCoordinateForView(float x, float y)
{
    float tempX = x;
    float tempY = y;

    if (x < 320)
        tempX = 320;
    if (x > 1728)
        tempX = 1728;
    if (y < 240)
        tempY = 240;
    if (y > 1808)
        tempY = 1808;

    view.setCenter(tempX, tempY);
}
#endif
