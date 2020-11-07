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

#ifndef METHODS_HPP
#define METHODS_HPP
////////////////////////////////////////////////////////////////// Statics
Texture Object::texture;
////////////////////////////////////////////////////////////////// Constructors
Player::Player(Image& image, int x, int y, int w, int h)
    : w{w},
      h{h},
      bonusClockRemain{false},
      bonusTime{0},
      bonusLeft{0},
      speed{1},
      score{0},
      life{true},
      bonusStart{false},
      eaten{false},
      winner{false}
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
Player1::Player1(Image& image, int x, int y, int w, int h)
    : Player(image, x, y, w, h)
{
}
Player2::Player2(Image& image, int x, int y, int w, int h)
    : Player(image, x, y, w, h)
{
    sprite.setColor(Color::Magenta);
}
Map::Map(Image& image)
{
    TileMap = new String[HEIGHT_MAP];
    TileMap[0] = TileMap[HEIGHT_MAP - 1] = "00000000000000000000000000000000";
    for (int i = 1; i < HEIGHT_MAP - 1; i++)
        TileMap[i] = "0                              0";
    texture.loadFromImage(image);
    sprite.setTexture(texture);
}
////////////////////////////////////////////////////////////////////// Gettters
Sprite& Object::getSprite()
{
    return sprite;
}
Texture& Object::getTexture()
{
    return texture;
}
Clock& Player::getClockMove()
{
    return clockMove;
}
Clock& Player::getClockBonus()
{
    return clockBonus;
}
bool& Player::getBonusClockRemain()
{
    return bonusClockRemain;
}
int& Player::getBonusTime()
{
    return bonusTime;
}
int Player::getW()
{
    return w;
}
int Player::getH()
{
    return h;
}
vector<Tile>& Player::getBody()
{
    return body;
}
float Player::getSpeed()
{
    return speed;
}
int Player::getScore()
{
    return score;
}
int Player::getBonusLeft()
{
    return bonusLeft;
}
bool& Player::getLife()
{
    return life;
}
bool Player::getBonusStart()
{
    return bonusStart;
}
bool Player::getEaten()
{
    return eaten;
}
bool& Player::getWinner()
{
    return winner;
}
String* Map::getMap()
{
    return TileMap;
}
////////////////////////////////////////////////////////////////////// Controls
void Player::control()
{
    if (Keyboard::isKeyPressed(Keyboard::A)
        && body[body.size() - 1].state != Tile::right) {
        body[0].state = Tile::left;
    } else if (
            Keyboard::isKeyPressed(Keyboard::D)
            && body[body.size() - 1].state != Tile::left) {
        body[0].state = Tile::right;
    } else if (
            Keyboard::isKeyPressed(Keyboard::W)
            && body[body.size() - 1].state != Tile::down) {
        body[0].state = Tile::up;
    } else if (
            Keyboard::isKeyPressed(Keyboard::S)
            && body[body.size() - 1].state != Tile::up) {
        body[0].state = Tile::down;
    }
}
void Player2::control()
{
    if (Keyboard::isKeyPressed(Keyboard::Left)
        && body[body.size() - 1].state != Tile::right) {
        body[0].state = Tile::left;
    } else if (
            Keyboard::isKeyPressed(Keyboard::Right)
            && body[body.size() - 1].state != Tile::left) {
        body[0].state = Tile::right;
    } else if (
            Keyboard::isKeyPressed(Keyboard::Up)
            && body[body.size() - 1].state != Tile::down) {
        body[0].state = Tile::up;
    } else if (
            Keyboard::isKeyPressed(Keyboard::Down)
            && body[body.size() - 1].state != Tile::up) {
        body[0].state = Tile::down;
    }
}
///////////////////////////////////////////////////////////////////// Growth
void Player::growth()
{
    Tile tile;
    tile.name = "Body";
    tile.x = body[0].x;
    tile.y = body[0].y;
    tile.state = body[0].state;
    body.push_back(tile);
}
///////////////////////////////////////////////////////////////// Map Collision
void Player::checkCollisionWithMap(Map& map)
{
    eaten = false;
    int i = body[0].y / TILE_SIZE;
    int j = body[0].x / TILE_SIZE;
    String* TileMap = map.getMap();
    if (TileMap[i][j] == '0') {
        life = false;
    } else if (TileMap[i][j] == '+') {
        speed = 2;
        bonusStart = true;
        TileMap[i][j] = ' ';
        map.randomMapGenerate('+', body);
    } else if (TileMap[i][j] == '-') {
        speed = 0.5;
        bonusStart = true;
        TileMap[i][j] = ' ';
        map.clearSlowers();
    } else if (TileMap[i][j] == 'a') {
        score++;
        eaten = true;
        TileMap[i][j] = ' ';
        growth();
        map.randomMapGenerate('a', body);
        map.randomMapGenerate('-', body);
    }
}
void Player::checkCollisionWithMap(Map& map, vector<Tile>& oppositeBody)
{
    eaten = false;
    int i = body[0].y / TILE_SIZE;
    int j = body[0].x / TILE_SIZE;
    String* TileMap = map.getMap();
    if (TileMap[i][j] == '0') {
        life = false;
    } else if (TileMap[i][j] == '+') {
        speed = 2;
        bonusStart = true;
        TileMap[i][j] = ' ';
        map.randomMapGenerate('+', body, oppositeBody);
    } else if (TileMap[i][j] == '-') {
        speed = 0.5;
        bonusStart = true;
        TileMap[i][j] = ' ';
        map.clearSlowers();
    } else if (TileMap[i][j] == 'a') {
        score++;
        eaten = true;
        TileMap[i][j] = ' ';
        growth();
        map.randomMapGenerate('a', body, oppositeBody);
        map.randomMapGenerate('-', body, oppositeBody);
    }
}
//////////////////////////////////////////////////////////////// Body Collision
void Player::checkCollisionWithBody()
{
    for (unsigned int i = 1; i < body.size(); i++)
        if (body[0].x == body[i].x && body[0].y == body[i].y)
            life = false;
}
void Player::checkCollisionWithBody(vector<Tile>& oppositeBody)
{
    checkCollisionWithBody();
    for (unsigned int i = 0; i < oppositeBody.size(); i++)
        if (body[0].x == oppositeBody[i].x && body[0].y == oppositeBody[i].y)
            life = false;
}
/////////////////////////////////////////////////////////////////////// Updates
bool Player::midUpdate(float currentMoveDelay)
{
    bool isMoved = false;
    if (life && currentMoveDelay * speed > MOVE_DELAY) {
        if (!eaten)
            for (unsigned int i = 1; i < body.size() - 1; i++) {
                body[i].x = body[i + 1].x;
                body[i].y = body[i + 1].y;
                body[i].state = body[i + 1].state;
            }
        body[body.size() - 1].x = body[0].x;
        body[body.size() - 1].y = body[0].y;
        switch (body[0].state) {
        case Tile::left:
            body[body.size() - 1].state = Tile::left;
            body[0].x -= TILE_SIZE;
            break;
        case Tile::right:
            body[body.size() - 1].state = Tile::right;
            body[0].x += TILE_SIZE;
            break;
        case Tile::up:
            body[body.size() - 1].state = Tile::up;
            body[0].y -= TILE_SIZE;
            break;
        case Tile::down:
            body[body.size() - 1].state = Tile::down;
            body[0].y += TILE_SIZE;
            break;
        }
        isMoved = true;
    }
    return isMoved;
}

bool Player::update(Map& map, int screenW, int screenH)
{
    float currentMoveDelay = clockMove.getElapsedTime().asMilliseconds();

    if (life)
        control();
    if (life && currentMoveDelay * speed > MOVE_DELAY) {
        checkCollisionWithMap(map);
    }
    bool isMoved = midUpdate(currentMoveDelay);
    if (life && currentMoveDelay * speed > MOVE_DELAY) {
        checkCollisionWithBody();
    }
    setPlayerCoordinateForView(screenW, screenH);
    return isMoved;
}
bool Player::update(
        Map& map, vector<Tile>& oppositeBody, int screenW, int screenH)
{
    float currentMoveDelay = clockMove.getElapsedTime().asMilliseconds();
    if (life)
        control();
    if (life && currentMoveDelay * speed > MOVE_DELAY) {
        checkCollisionWithMap(map, oppositeBody);
    }
    bool isMoved = midUpdate(currentMoveDelay);
    if (life && currentMoveDelay * speed > MOVE_DELAY) {
        checkCollisionWithBody(oppositeBody);
    }
    setPlayerCoordinateForView(screenW, screenH);
    return isMoved;
}
//////////////////////////////////////////////////////////////// Bonus setter
void Player::setBonusTimer()
{
    int currentBonusTime = 0;
    if (bonusStart) {
        bonusTime = BONUS_TIME;
        bonusClockRemain = true;
        clockBonus.restart();
        bonusStart = false;
    }
    if (bonusClockRemain)
        currentBonusTime = clockBonus.getElapsedTime().asSeconds();
    else
        bonusTime = 0;
    if (bonusTime > 0)
        bonusLeft = 10 - currentBonusTime;
    if (currentBonusTime >= bonusTime) {
        speed = 1;
        bonusTime = 0;
        bonusClockRemain = false;
    }
}
////////////////////////////////////////////////////////////// Drawers
void Player::draw()
{
    for (unsigned int i = 0; i < body.size(); i++) {
        if (body[i].name == "Head") {
            sprite.setTextureRect(IntRect(0, 0, TILE_SIZE, TILE_SIZE));
        } else if (body[i].name == "Body") {
            if (body[i].state == body[i - 1].state) {
                sprite.setTextureRect(
                        IntRect(TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
            } else if (
                    (body[i].state == Tile::left
                     && body[i - 1].state == Tile::up)
                    || (body[i].state == Tile::up
                        && body[i - 1].state == Tile::right)
                    || (body[i].state == Tile::right
                        && body[i - 1].state == Tile::down)
                    || (body[i].state == Tile::down
                        && body[i - 1].state == Tile::left)) {
                sprite.setTextureRect(
                        IntRect(TILE_SIZE * 3, 0, TILE_SIZE, TILE_SIZE));
            } else {
                sprite.setTextureRect(
                        IntRect(TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE));
            }
        } else if (body[i].name == "Tail") {
            sprite.setTextureRect(
                    IntRect(TILE_SIZE * 4, 0, TILE_SIZE, TILE_SIZE));
        }
        switch (body[i].state) {
        case Tile::left:
            sprite.setRotation(180);
            break;
        case Tile::right:
            sprite.setRotation(0);
            break;
        case Tile::up:
            sprite.setRotation(270);
            break;
        case Tile::down:
            sprite.setRotation(90);
            break;
        }
        sprite.setPosition(body[i].x + w / 2, body[i].y + h / 2);
        window.draw(sprite);
        sprite.setRotation(0);
    }
}
void Player::draw(int screenW, int screenH)
{
    Text scoreText("", font, screenW / 80 + TILE_SIZE / 2),
            bonusText("", font, screenW / 80 + TILE_SIZE / 2);
    scoreText.setFillColor(Color(77, 64, 37));
    bonusText.setFillColor(Color(77, 64, 37));

    ostringstream scoreStream, bonusStream;
    scoreStream << setfill('0') << setw(2) << score;

    scoreText.setString(L"Яблок собрано: " + scoreStream.str());
    scoreText.setPosition(
            view[0].getCenter().x - screenW * 15 / 32 + TILE_SIZE,
            view[0].getCenter().y - screenH / 2 + TILE_SIZE);
    window.draw(scoreText);
    if (life && bonusTime > 0) {
        bonusStream << setfill('0') << setw(2) << bonusLeft;
        bonusText.setString(L"Действие бонуса: " + bonusStream.str());
        bonusText.setPosition(
                view[0].getCenter().x - screenW * 15 / 32 + TILE_SIZE,
                view[0].getCenter().y - screenH / 2 + 2 * TILE_SIZE);
        window.draw(bonusText);
    } else if (!life) {
        sprite.setColor(Color::Black);
        gameOver.setPosition(
                view[0].getCenter().x - screenW / 4,
                view[0].getCenter().y - screenH / 4);
        window.draw(gameOver);
    }
}
void Map::draw()
{
    for (int i = 0; i < HEIGHT_MAP; i++)
        for (int j = 0; j < WIDTH_MAP; j++) {
            if (TileMap[i][j] == ' ') {
                sprite.setTextureRect(
                        IntRect(TILE_SIZE * 5, 0, TILE_SIZE, TILE_SIZE));
            }
            if (TileMap[i][j] == 'a') {
                sprite.setTextureRect(
                        IntRect(TILE_SIZE * 7, 0, TILE_SIZE, TILE_SIZE));
            }
            if (TileMap[i][j] == '0') {
                sprite.setTextureRect(
                        IntRect(TILE_SIZE * 6, 0, TILE_SIZE, TILE_SIZE));
            }
            if (TileMap[i][j] == '+') {
                sprite.setTextureRect(
                        IntRect(TILE_SIZE * 9, 0, TILE_SIZE, TILE_SIZE));
            }
            if (TileMap[i][j] == '-') {
                sprite.setTextureRect(
                        IntRect(TILE_SIZE * 8, 0, TILE_SIZE, TILE_SIZE));
            }
            sprite.setPosition(j * TILE_SIZE, i * TILE_SIZE);
            window.draw(sprite);
        }
}
////////////////////////////////////////////////////////////// View coordinates
void Player::setTemps(float& tempX, float& tempY, int halfW, int halfH)
{
    if (body[0].x < halfW)
        tempX = halfW;
    if (body[0].x > TILE_SIZE * WIDTH_MAP - halfW)
        tempX = TILE_SIZE * WIDTH_MAP - halfW;
    if (body[0].y < halfH)
        tempY = halfH;
    if (body[0].y > TILE_SIZE * HEIGHT_MAP - halfH)
        tempY = TILE_SIZE * HEIGHT_MAP - halfH;
}

void Player::setPlayerCoordinateForView(int screenW, int screenH)
{
    float tempX = body[0].x, tempY = body[0].y;
    int halfW = screenW / 2, halfH = screenH / 2;

    setTemps(tempX, tempY, halfW, halfH);

    view[0].setCenter(
            view[0].getCenter().x
                    + (tempX - view[0].getCenter().x) / (halfW / 20 / speed),
            view[0].getCenter().y
                    + (tempY - view[0].getCenter().y) / (halfH / 20 / speed));
}
void Player1::setPlayerCoordinateForView(int screenW, int screenH)
{
    float tempX = body[0].x, tempY = body[0].y;
    int halfW = screenW / 4 - SEPARATOR_WIDTH / 2, halfH = screenH / 2;

    setTemps(tempX, tempY, halfW, halfH);

    view[0].setCenter(
            view[0].getCenter().x
                    + (tempX - view[0].getCenter().x) / (halfW / 20 / speed),
            view[0].getCenter().y
                    + (tempY - view[0].getCenter().y) / (halfH / 20 / speed));
}
void Player2::setPlayerCoordinateForView(int screenW, int screenH)
{
    float tempX = body[0].x, tempY = body[0].y;
    int halfW = screenW / 4 - SEPARATOR_WIDTH / 2, halfH = screenH / 2;

    setTemps(tempX, tempY, halfW, halfH);

    view[1].setCenter(
            view[1].getCenter().x
                    + (tempX - view[1].getCenter().x) / (halfW / 20 / speed),
            view[1].getCenter().y
                    + (tempY - view[1].getCenter().y) / (halfH / 20 / speed));
}

void Player::changeView(int screenW, int screenH)
{
    view[0].setSize(screenW, screenH);
    view[0].setViewport(FloatRect(0, 0, 1, 1));
    view[0].setCenter(screenW / 2, screenH / 2);
    view[1].setSize(0, 0);
}
void Player1::changeView(int screenW, int screenH)
{
    view[0].setSize(screenW / 2 - SEPARATOR_WIDTH, screenH);
    view[0].setViewport(FloatRect(
            0, 0, float((screenW / 2 - SEPARATOR_WIDTH)) / float(screenW), 1));
    view[0].setCenter(screenW / 4 - SEPARATOR_WIDTH / 2, screenH / 2);
}
void Player2::changeView(int screenW, int screenH)
{
    view[1].setSize(screenW / 2 - SEPARATOR_WIDTH, screenH);
    view[1].setViewport(FloatRect(
            float(1 - float((screenW / 2 - SEPARATOR_WIDTH)) / float(screenW)),
            0,
            float((screenW / 2 - SEPARATOR_WIDTH)) / float(screenW),
            1));
    view[1].setCenter(screenW / 4 * 3 - SEPARATOR_WIDTH / 2, screenH / 2);
}
/////////////////////////////////////////////////////////// Random Map Generate
void Map::randomMapGenerate(
        char bonusType, vector<Tile>& playerBody, int countBonus)
{
    srand(time(0));
    while (countBonus > 0) {
        bool generate = true;
        int randomElementX = 1 + rand() % (WIDTH_MAP - 1);
        int randomElementY = 1 + rand() % (HEIGHT_MAP - 1);
        for (unsigned int i = 0; i < playerBody.size(); i++) {
            if (playerBody[i].x == randomElementX * TILE_SIZE
                && playerBody[i].y == randomElementY * TILE_SIZE)
                generate = false;
        }
        if (TileMap[randomElementY][randomElementX] == ' ' && generate) {
            TileMap[randomElementY][randomElementX] = bonusType;
            countBonus--;
        }
    }
}
void Map::randomMapGenerate(
        char bonusType,
        vector<Tile>& playerBody,
        vector<Tile>& oppositeBody,
        int countBonus)
{
    srand(time(0));
    while (countBonus > 0) {
        bool generate = true;
        int randomElementX = 1 + rand() % (WIDTH_MAP - 1);
        int randomElementY = 1 + rand() % (HEIGHT_MAP - 1);
        for (unsigned int i = 0; i < playerBody.size(); i++) {
            if (playerBody[i].x == randomElementX * TILE_SIZE
                && playerBody[i].y == randomElementY * TILE_SIZE)
                generate = false;
        }
        for (unsigned int i = 0; i < oppositeBody.size(); i++) {
            if (oppositeBody[i].x == randomElementX * TILE_SIZE
                && oppositeBody[i].y == randomElementY * TILE_SIZE)
                generate = false;
        }
        if (TileMap[randomElementY][randomElementX] == ' ' && generate) {
            TileMap[randomElementY][randomElementX] = bonusType;
            countBonus--;
        }
    }
}
///////////////////////////////////////////////////////////// Map Clear Methods
void Map::clearSlowers(int count)
{
    for (int i = 1; i < WIDTH_MAP - 1; i++)
        for (int j = 1; j < HEIGHT_MAP - 1; j++)
            if (TileMap[j][i] == '-' && count != 0) {
                TileMap[j][i] = ' ';
                if (count > 0)
                    count--;
            }
}
void Map::clearMap()
{
    for (int i = 1; i < WIDTH_MAP - 1; i++)
        for (int j = 1; j < HEIGHT_MAP - 1; j++)
            if (TileMap[j][i] != '0')
                TileMap[j][i] = ' ';
}

#endif
