#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <stdlib.h>
using namespace sf;

#ifndef MAP_HPP
#define MAP_HPP

const int HEIGHT_MAP = 32;
const int WIDTH_MAP = 32;

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

void clearStoppers()
{
    for (int i = 1; i < WIDTH_MAP - 1; i++) {
        for (int j = 1; j < HEIGHT_MAP - 1; j++) {
            if (TileMap[j][i] == '-') {
                TileMap[j][i] = ' ';
            }
        }
    }
}
void clearMap()
{
    for (int i = 1; i < WIDTH_MAP - 1; i++) {
        for (int j = 1; j < HEIGHT_MAP - 1; j++) {
            if (TileMap[j][i] != '0') {
                TileMap[j][i] = ' ';
            }
        }
    }
}
#endif
