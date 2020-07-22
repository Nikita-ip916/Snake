#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <windows.h>

using namespace std;
using namespace sf;

#ifndef PLAYER_HPP
#define PLAYER_HPP
class Player {
public:
    struct Tile {
        string name;
        enum { left, right, up, down } state;
        int x;
        int y;
    } tile;
    vector<tile> body;
    float x, y, dx, dy, speed, moveTimer;
    int w, h, size, score;
    bool life;
    Texture texture;
    Sprite sprite;
    String name;
    Player(Image& image, String Name, float X, float Y, float W, float H)
    {
        x = tile.x = X;
        y = tile.y = Y;
        w = W;
        h = H;
        name = Name;
        tile.name = "Head";
        tile.state = right;
        body.push_back(tile);
        tile.x = X - W;
        tile.name = "Tail";
        tile.state = right;
        body.push_back(tile);
        score = moveTimer = 0;
        speed = 0.5;
        dx = dy = 0;
        life = true;
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setOrigin(w / 2, h / 2);
        if (name == "Player2") {
            sprite.setColor(Color::Red);
        }
    }
}
#endif
