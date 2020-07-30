#include <SFML/Graphics.hpp>
using namespace sf;
#ifndef VIEW_HPP
#define VIEW_HPP
View view;

void setPlayerCoordinateForView(float x, float y)
{
    float tempX = x;
    float tempY = y;

    /*if (x < 272) tempX = 272;
    if (x > 912) tempX = 912;
    if (y < 192) tempY = 192;
    if (y > 624) tempY = 624;*/

    view.setCenter(tempX, tempY);
}
#endif
