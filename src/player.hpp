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
View view, viewP2;
const int HEIGHT_MAP = 32;
const int WIDTH_MAP = 32;
const int TILE_SIZE = 64;
const int SEPARATOR_WIDTH = 12;
const float MOVE_DELAY = 600;
const int BONUS_TIME = 10;
const int DEFAULT_TEXT_SIZE = 50;

struct Tile {
    string name;
    enum { left, right, up, down } state;
    int x;
    int y;
};

class Object {
protected:
    static Texture texture;
    static Sprite sprite;

public:
    static Sprite& getSprite();
    static Texture& getTexture();
};

class Map : public Object {
protected:
    String TileMap[HEIGHT_MAP];

public:
    Map();
    void randomMapGenerate(char bonusType, vector<Tile>& body, int countBonus);
    void randomMapGenerate(
            char bonusType,
            vector<Tile>& body,
            vector<Tile>& oppositeBody,
            int countBonus);
    void clearSlowers(int count);
    void clearMap();
    String*& getMap();

    void draw();
};

class Player : public Object {
protected:
    Clock clockMove, clockBonus;
    bool bonusClockRemain;
    // рабочие ostringstream bonus, score;
    // рабочие Text bonusText, scoreText;
    // рабочий float currentMoveDelay; +++
    // рабочий int currentBonusTime; +++
    int bonusTime;
    int w, h;
    vector<Tile> body;
    float speed;
    int score;
    bool life, bonusStart, eaten, winner;
    bool midUpdate(float currentMoveDelay);
    void setTemps(int& tempX, int& tempY, int halfW, int halfH);

public:
    Player(Image& image, int x, int y, int w, int h);
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
    bool getLife();
    bool getBonusStart();
    bool getEaten();
    bool getWinner();

    void growth();
    void checkCollisionWithMap(Map& map);
    void checkCollisionWithMap(Map& map, vector<Tile>& oppositeBody);
    void checkCollisionWithBody();
    void checkCollisionWithBody(vector<Tile>& oppositeBody);
    bool update(Map& map);
    bool update(Map& map, vector<Tile>& oppositeBody);

    void setBonusTimer();

    void draw();
    void draw(int screenW, int screenH);
};

class Player1 : public Player {
public:
    Player1(Image& image, int x, int y, int w, int h);
    virtual void changeView(int screenW, int screenH);
    virtual void setPlayerCoordinateForView(int screenW, int screenH);
};

class Player2 : public Player {
public:
    Player2(Image& image, int x, int y, int w, int h);
    virtual void control();
    virtual void changeView(int screenW, int screenH);
    virtual void setPlayerCoordinateForView(int screenW, int screenH);
};

#include "methods.hpp"
#endif
