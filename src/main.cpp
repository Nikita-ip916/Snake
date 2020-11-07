#include "display.hpp"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
int main()
{
    if (!checkResourse("images/tiles.png")
        || !checkResourse("images/PostModern.ttf")) {
        return 1;
    }
    display();
    return 0;
}
