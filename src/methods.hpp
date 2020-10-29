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
Sprite Object::sprite;
////////////////////////////////////////////////////////////////// Constructors
Player::Player(Image& image, int x, int y, int w = TILE_SIZE, int h = TILE_SIZE)
    : score(0),
      speed(1),
      w(w),
      h(h),
      bonusStart(false),
      eaten(false),
      winner(false),
      life(true),
      bonusClockRemain(false)
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
Player1::Player1(
        Image& image, int x, int y, int w = TILE_SIZE, int h = TILE_SIZE)
{
    Player(image, x, y, w, h);
}
Player2::Player2(
        Image& image, int x, int y, int w = TILE_SIZE, int h = TILE_SIZE)
{
    Player(image, x, y, w, h);
    sprite.setColor(Color::Magenta);
}
Map::Map(Image& image)
{
    TileMap = {
            "00000000000000000000000000000000",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "0                              0",
            "00000000000000000000000000000000",
    };
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
bool Player::getLife()
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
bool Player::getWinner()
{
    return winner;
}
String*& Map::getMap()
{
    return TileMap[];
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
    int i = body[0].y / 64;
    int j = body[0].x / 64;
    String TileMap = map.getMap();
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
    int i = body[0].y / 64;
    int j = body[0].x / 64;
    String TileMap = map.getMap();
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
}

bool Player::update(Map& map)
{
    float currentMoveDelay = clockMove.getElapsedTime().asMilliseconds();

    if (life)
        control();
    if (life && currentMoveDelay * speed > MOVE_DELAY) {
        checkCollisionWithMap(map);
        checkCollisionWithBody();
    }
    bool isMoved = midUpdate(currentMoveDelay);
    setPlayerCoordinateForView();
    return isMoved;
}
bool Player::update(Map& map, vector<Tile>& oppositeBody)
{
    float currentMoveDelay = clockMove.getElapsedTime().asMilliseconds();
    if (life)
        control();
    if (life && currentMoveDelay * speed > MOVE_DELAY) {
        checkCollisionWithMap(map, oppositeBody);
        checkCollisionWithBody(oppositeBody);
    }
    bool isMoved = midUpdate(currentMoveDelay);
    setPlayerCoordinateForView();
    return isMoved;
}
//////////////////////////////////////////////////////////////// Bonus setter
void Player::setBonusTimer()
{
    if (bonusStart) {
        bonusTime = BONUS_TIME;
        bonusClockRemain = true;
        clockBonus.restart();
        bonusStart = false;
    }
    if (bonusClockRemain) {
        currentBonusTime = clockBonus.getElapsedTime().asSeconds();
    } else {
        bonusTime = 0;
    }
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
            sprite.setTextureRect(IntRect(0, 0, 64, 64));
        } else if (body[i].name == "Body") {
            if (body[i].state == body[i - 1].state) {
                sprite.setTextureRect(IntRect(64, 0, 64, 64));
            } else if (
                    (body[i].state == Player::Tile::left
                     && body[i - 1].state == Player::Tile::up)
                    || (body[i].state == Player::Tile::up
                        && body[i - 1].state == Player::Tile::right)
                    || (body[i].state == Player::Tile::right
                        && body[i - 1].state == Player::Tile::down)
                    || (body[i].state == Player::Tile::down
                        && body[i - 1].state == Player::Tile::left)) {
                sprite.setTextureRect(IntRect(192, 0, 64, 64));
            } else {
                sprite.setTextureRect(IntRect(128, 0, 64, 64));
            }
        } else if (body[i].name == "Tail") {
            sprite.setTextureRect(IntRect(256, 0, 64, 64));
        }
        switch (body[i].state) {
        case Player::Tile::left:
            sprite.setRotation(180);
            break;
        case Player::Tile::right:
            sprite.setRotation(0);
            break;
        case Player::Tile::up:
            sprite.setRotation(270);
            break;
        case Player::Tile::down:
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
    screenW = 30;
    screenH = 15;
    Text scoreText("", font, DEFAULT_TEXT_SIZE),
            bonusText("", font, DEFAULT_TEXT_SIZE);
    scoreText.setFillColor(Color(77, 64, 37));
    bonusText.setFillColor(Color(77, 64, 37));

    ostringstream scoreStream, bonusStream;
    scoreStream << setfill('0') << setw(2) << score;

    scoreText.setString(L"Яблок собрано: " + scoreStream.str());
    // score[1].str("");
    scoreText.setPosition(view.getCenter().x - 890, view.getCenter().y - 470);
    window.draw(scoreText);
    if (life && bonusTime > 0) {
        bonusStream << setfill('0') << setw(2) << 10 - currentBonusTime;
        bonusText.setString(L"Действие бонуса: " + bonusStream.str());
        bonus[1].str("");
        bonusText.setPosition(
                view.getCenter().x - 890, view.getCenter().y - 406);
        window.draw(bonusText);
    } else if (!life) {
        sprite.setColor(Color::Black);
        gameOver.setPosition(
                view.getCenter().x - 450, view.getCenter().y - 150);
        window.draw(gameOver);
    }
}
void Map::draw()
{
    for (int i = 0; i < HEIGHT_MAP; i++)
        for (int j = 0; j < WIDTH_MAP; j++) {
            if (TileMap[i][j] == ' ') {
                sprite.setTextureRect(IntRect(320, 0, 64, 64));
            }
            if (TileMap[i][j] == 'a') {
                sprite.setTextureRect(IntRect(448, 0, 64, 64));
            }
            if (TileMap[i][j] == '0') {
                sprite.setTextureRect(IntRect(384, 0, 64, 64));
            }
            if (TileMap[i][j] == '+') {
                sprite.setTextureRect(IntRect(576, 0, 64, 64));
            }
            if (TileMap[i][j] == '-') {
                sprite.setTextureRect(IntRect(512, 0, 64, 64));
            }
            sprite.setPosition(j * 64, i * 64);
            window.draw(sprite);
        }
}
////////////////////////////////////////////////////////////// View coordinates
void Player::setTemps(int& tempX, int& tempY, int halfW, int halfH)
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

    view.setCenter(
            view.getCenter().x
                    + (tempX - view.getCenter().x) / (halfW / 20 / speed),
            view.getCenter().y
                    + (tempY - view.getCenter().y) / (halfH / 20 / speed));
}
void Player1::setPlayerCoordinateForView(int screenW, int screenH)
{
    float tempX = body[0].x, tempY = body[0].y;
    int halfW = screenW / 4 - SEPARATOR_WIDTH / 2, halfH = screenH / 2;

    setTemps(tempX, tempY, halfW, halfH);

    view.setCenter(
            view.getCenter().x
                    + (tempX - view.getCenter().x) / (halfW / 20 / speed),
            view.getCenter().y
                    + (tempY - view.getCenter().y) / (halfH / 20 / speed));
}
void Player2::setPlayerCoordinateForView(int screenW, int screenH)
{
    float tempX = body[0].x, tempY = body[0].y;
    int halfW = screenW / 4 - SEPARATOR_WIDTH / 2, halfH = screenH / 2;

    setTemps(tempX, tempY, halfW, halfH);

    viewP2.setCenter(
            viewP2.getCenter().x
                    + (tempX - viewP2.getCenter().x) / (halfW / 20 / speed),
            viewP2.getCenter().y
                    + (tempY - viewP2.getCenter().y) / (halfH / 20 / speed));
}

void Player::changeView(int screenW, int screenH)
{
    view.setSize(screenW, screenH);
    view.setViewport(FloatRect(0, 0, 1, 1));
    view.setCenter(screenW / 2, screenH / 2);
    viewP2.setSize(0, 0);
}
void Player1::changeView(int screenW, int screenH)
{
    view.setSize(screenW / 2 - SEPARATOR_WIDTH, screenH);
    view.setViewport(
            FloatRect(0, 0, (screenW / 2 - SEPARATOR_WIDTH) / screenW, 1));
    view.setCenter(screenW / 4 - SEPARATOR_WIDTH / 2, screenH / 2);
}
void Player2::changeView(int screenW, int screenH)
{
    viewP2.setSize(screenW / 2 - SEPARATOR_WIDTH, screenH);
    viewP2.setViewport(FloatRect(
            1 - (screenW / 2 - SEPARATOR_WIDTH) / screenW,
            0,
            (screenW / 2 - SEPARATOR_WIDTH) / screenW,
            1));
    viewP2.setCenter(screenW / 4 - SEPARATOR_WIDTH / 2, screenH / 2);
}
/////////////////////////////////////////////////////////// Random Map Generate
void Map::randomMapGenerate(
        char bonusType, vector<Tile>& playerBody, int countBonus = 1)
{
    srand(time(0));
    while (countBonus > 0) {
        bool generate = true;
        int randomElementX = 1 + rand() % (WIDTH_MAP - 1);
        int randomElementY = 1 + rand() % (HEIGHT_MAP - 1);
        for (unsigned int i = 0; i < body.size(); i++) {
            if (playerBody[i].x == randomElementX * 64
                && playerBody[i].y == randomElementY * 64)
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
        int countBonus = 1)
{
    srand(time(0));
    while (countBonus > 0) {
        bool generate = true;
        int randomElementX = 1 + rand() % (WIDTH_MAP - 1);
        int randomElementY = 1 + rand() % (HEIGHT_MAP - 1);
        for (unsigned int i = 0; i < body.size(); i++) {
            if (playerBody[i].x == randomElementX * 64
                && playerBody[i].y == randomElementY * 64)
                generate = false;
        }
        for (unsigned int i = 0; i < oppositeBody.size(); i++) {
            if (oppositeBody[i].x == randomElementX * 64
                && oppositeBody[i].y == randomElementY * 64)
                generate = false;
        }
        if (TileMap[randomElementY][randomElementX] == ' ' && generate) {
            TileMap[randomElementY][randomElementX] = bonusType;
            countBonus--;
        }
    }
}
///////////////////////////////////////////////////////////// Map Clear Methods
void Map::clearSlowers(int count = -1)
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
