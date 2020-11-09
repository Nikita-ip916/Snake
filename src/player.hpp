#include <SFML/Graphics.hpp>
#include <iomanip>
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

RenderWindow window;
Font font;
Text gameOver;
Text gameWon;
View view[2];
const int HEIGHT_MAP = 32;
const int WIDTH_MAP = 32;
const int TILE_SIZE = 64;
const int SEPARATOR_WIDTH = 12;
const float MOVE_DELAY = 600;
const int BONUS_TIME = 10;
const int WIN_SCORE = 5;

struct Tile {
    string name;
    enum { left, right, up, down } state;
    int x;
    int y;
};

class Object {
protected:
    static Texture texture;
    Sprite sprite;

public:
    Sprite& getSprite();
    static Texture& getTexture();
};

class Map : public Object {
protected:
    String* TileMap;

public:
    Map(Image& image);
    void randomMapGenerate(
            char bonusType, vector<Tile>& playerBody, int countBonus = 1);
    void randomMapGenerate(
            char bonusType,
            vector<Tile>& playerBody,
            vector<Tile>& oppositeBody,
            int countBonus = 1);
    void clearSlowers(int count = -1);
    void clearMap();
    String* getMap();

    void draw();
};

class Player : public Object {
protected:
    Clock clockMove, clockBonus;
    int w, h;
    bool bonusClockRemain;
    int bonusTime, bonusLeft;
    float speed;
    int score;
    bool life, bonusStart, eaten, winner;
    vector<Tile> body;
    bool midUpdate(float currentMoveDelay);
    void setTemps(float& tempX, float& tempY, int halfW, int halfH);

public:
    Player(Image& image, int x, int y, int w = TILE_SIZE, int h = TILE_SIZE);
    virtual void control();
    virtual void changeView(int screenW, int screenH);
    virtual void setPlayerCoordinateForView(int screenW, int screenH);

    Clock& getClockMove();
    Clock& getClockBonus();
    bool& getBonusClockRemain();
    int& getBonusTime();

    int getW();
    int getH();
    vector<Tile>& getBody();
    float getSpeed();
    int getScore();
    int getBonusLeft();
    bool& getLife();
    bool getBonusStart();
    bool getEaten();
    bool& getWinner();

    void growth();
    void checkCollisionWithMap(Map& map);
    void checkCollisionWithMap(Map& map, vector<Tile>& oppositeBody);
    void checkCollisionWithBody();
    void checkCollisionWithBody(vector<Tile>& oppositeBody);
    bool update(Map& map, int screenW, int screenH);
    bool update(Map& map, vector<Tile>& oppositeBody, int screenW, int screenH);

    void setBonusTimer();

    void draw();
    void draw(int screenW, int screenH);
};

class Player1 : public Player {
public:
    Player1(Image& image, int x, int y, int w = TILE_SIZE, int h = TILE_SIZE);
    virtual void changeView(int screenW, int screenH);
    virtual void setPlayerCoordinateForView(int screenW, int screenH);
};

class Player2 : public Player {
public:
    Player2(Image& image, int x, int y, int w = TILE_SIZE, int h = TILE_SIZE);
    virtual void control();
    virtual void changeView(int screenW, int screenH);
    virtual void setPlayerCoordinateForView(int screenW, int screenH);
};

#include "methods.hpp"
#endif
