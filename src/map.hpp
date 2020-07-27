#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
using namespace sf;

#ifndef MAP_HPP
#define MAP_HPP

const int HEIGHT_MAP = 25;
const int WIDTH_MAP = 40;

String TileMap[HEIGHT_MAP] = {
        "00000000000000000000000000000000", "0                              0",
        "0                              0", "0                              0",
        "0                              0", "0                              0",
        "0                              0", "0                              0",
        "0                              0", "0                              0",
        "0                              0", "0                              0",
        "0                              0", "0                              0",
        "0                              0", "0                              0",
        "0                              0", "0                              0",
        "0                              0", "0                              0",
        "0                              0", "0                              0",
        "0                              0", "0                              0",
        "0                              0", "0                              0",
        "0                              0", "0                              0",
        "0                              0", "0                              0",
        "0                              0", "00000000000000000000000000000000",
};

void randomMapGenerate(String bonus)
{
    int randomElementX = 0;
    int randomElementY = 0;
    srand(time(0));
    int countBonus = 1;
    while (countBonus > 0) {
        randomElementX = 1 + rand() % (WIDTH_MAP - 1);
        randomElementY = 1 + rand() % (HEIGHT_MAP - 1);
        if (TileMap[randomElementY][randomElementX] == ' ') {
            TileMap[randomElementY][randomElementX] = bonus;
            countBonus--;
        }
    }
}
