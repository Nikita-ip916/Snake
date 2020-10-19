#include "map.hpp"
#include "view.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;
using namespace sf;

#ifndef PLAYER_HPP
#define PLAYER_HPP

struct Tile {
    string name;
    enum { left, right, up, down } state;
    int x;
    int y;
};

class Object {
protected:
    int w, h;
    Texture texture;
    Sprite sprite;

public:
};

class Player : public Object {
protected:
    vector<Tile> body;
    enum { wasd, arrows } controls;
    float speed;
    int score;
    bool life, bonusTimer, eaten, winner;
    Player(Image& image, int x, int y, int w = TILE_SIZE, int h = TILE_SIZE)
        : score(0),
          speed(1),
          w(w),
          h(h),
          bonusTimer(false),
          eaten(false),
          winner(false),
          life(true),
          controls(wasd)
    {
        Tile tile;
        tile.x = x;
        tile.y = y;
        tile.name = "Head";
        tile.state = Tile::right;
        body.push_back(tile);
        tile.x = x - w;
        tile.name = "Tail";
        tile.state = Tile::right;
        body.push_back(tile);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setOrigin(w / 2, h / 2);
    }
    bool midUpdate(float currentMoveDelay);
    void setTemps(int& tempX, int& tempY);

public:
    virtual void control();
    virtual void setPlayerCoordinateForView(int screenW, int screenH);
    void growth();
    void checkCollisionWithMap(Bonus booster, Bonus slower, Bonus apple);
    void checkCollisionWithMap(
            Bonus booster,
            Bonus slower,
            Bonus apple,
            vector<Tile>& oppositeBody);
    void checkCollisionWithBody();
    void checkCollisionWithBody(vector<Tile> oppositeBody);
    bool update(float currentMoveDelay);
    bool update(float currentMoveDelay, vector<Tile> oppositeBody);
    void setBodySprite(unsigned int i);
};

class Player1 : public Player {
public:
    Player1(Image& image, int x, int y, int w, int h)
    {
        Player(image, x, y, w, h);
    }
    virtual void setPlayerCoordinateForView(int screenW, int screenH);
};

class Player2 : public Player {
public:
    Player2(Image& image, int x, int y, int w, int h) controls(arrows)
    {
        Player(image, x, y, w, h);
        sprite.setColor(Color::Magenta);
    }
    virtual void control();
    virtual void setPlayerCoordinateForView(int screenW, int screenH);
};

class Bonus : public Object {
protected:
    char bonusType;

public:
    Bonus(char bonusType) : bonusType(bonusType);
    void randomMapGenerate(vector<Tile>& body, vector<Tile>& oppositeBody);
}

#endif
