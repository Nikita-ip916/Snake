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
class Player {
public:
    struct Tile {
        string name;
        enum { left, right, up, down } state;
        int x;
        int y;
    };
    enum { wasd, arrows } controls;
    vector<Tile> body, oppositeBody;
    Tile tile;
    float x, y, speed;
    int w, h, score;
    bool life, bonusTimer, eaten, winner;
    Texture texture;
    Sprite sprite;
    string name, plNumber;
    Player(Image& image,
           string PlNumber,
           string Name,
           float X,
           float Y,
           float W,
           float H)
    {
        x = tile.x = X;
        y = tile.y = Y;
        w = W;
        h = H;
        plNumber = PlNumber;
        name = Name;
        tile.name = "Head";
        tile.state = Tile::right;
        body.push_back(tile);
        tile.x = X - W;
        tile.name = "Tail";
        tile.state = Tile::right;
        body.push_back(tile);
        score = 0;
        speed = 1;
        bonusTimer = eaten = winner = false;
        life = true;
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setOrigin(w / 2, h / 2);
        if (name == "Player1") {
            controls = wasd;
        } else if (name == "Player2") {
            sprite.setColor(Color::Magenta);
            controls = arrows;
        }
    }
    void control()
    {
        if (controls == wasd) {
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
        } else if (controls == arrows) {
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
    }
    void growth()
    {
        tile.name = "Body";
        tile.x = body[0].x;
        tile.y = body[0].y;
        tile.state = body[0].state;
        body.push_back(tile);
    }
    void randomMapGenerate(char bonus)
    {
        int randomElementX = 0;
        int randomElementY = 0;
        bool generate;
        srand(time(0));
        int countBonus = 1;
        while (countBonus > 0) {
            generate = true;
            randomElementX = 1 + rand() % (WIDTH_MAP - 1);
            randomElementY = 1 + rand() % (HEIGHT_MAP - 1);
            for (unsigned int i = 0; i < body.size(); i++) {
                if (body[i].x == randomElementX * 64
                    && body[i].y == randomElementY * 64)
                    generate = false;
            }
            for (unsigned int i = 0; i < oppositeBody.size(); i++) {
                if (oppositeBody[i].x == randomElementX * 64
                    && oppositeBody[i].y == randomElementY * 64)
                    generate = false;
            }
            if (TileMap[randomElementY][randomElementX] == ' ' && generate) {
                TileMap[randomElementY][randomElementX] = bonus;
                countBonus--;
            }
        }
    }
    void checkCollisionWithMap()
    {
        eaten = false;
        for (int i = body[0].y / 64; i < (body[0].y + h) / 64; i++)
            for (int j = body[0].x / 64; j < (body[0].x + w) / 64; j++) {
                if (TileMap[i][j] == '0') {
                    life = false;
                } else if (TileMap[i][j] == '+') {
                    speed = 2;
                    bonusTimer = true;
                    TileMap[i][j] = ' ';
                    randomMapGenerate('+');
                } else if (TileMap[i][j] == '-') {
                    speed = 0.5;
                    bonusTimer = true;
                    TileMap[i][j] = ' ';
                    clearStoppers();
                } else if (TileMap[i][j] == 'a') {
                    score++;
                    eaten = true;
                    TileMap[i][j] = ' ';
                    growth();
                    randomMapGenerate('a');
                    randomMapGenerate('+');
                }
            }
    }
    void checkCollisionWithBody()
    {
        for (unsigned int i = 1; i < body.size(); i++)
            if (body[0].x == body[i].x && body[0].y == body[i].y)
                life = false;
        for (unsigned int i = 0; i < oppositeBody.size(); i++)
            if (body[0].x == oppositeBody[i].x
                && body[0].y == oppositeBody[i].y)
                life = false;
    }
    bool update(float currentMoveDelay)
    {
        bool isMoved = false;
        float moveDelay = 600;
        if (life) {
            control();
            if (currentMoveDelay * speed > moveDelay) {
                checkCollisionWithMap();
                if (life) {
                    if (!eaten)
                        for (unsigned int i = 1; i < body.size() - 1; i++) {
                            body[i].x = body[i + 1].x;
                            body[i].y = body[i + 1].y;
                            body[i].state = body[i + 1].state;
                        }
                    switch (body[0].state) {
                    case Tile::left:
                        body[body.size() - 1].x = body[0].x;
                        body[body.size() - 1].y = body[0].y;
                        body[body.size() - 1].state = Tile::left;
                        body[0].x -= 64;
                        break;
                    case Tile::right:
                        body[body.size() - 1].x = body[0].x;
                        body[body.size() - 1].y = body[0].y;
                        body[body.size() - 1].state = Tile::right;
                        body[0].x += 64;
                        break;
                    case Tile::up:
                        body[body.size() - 1].x = body[0].x;
                        body[body.size() - 1].y = body[0].y;
                        body[body.size() - 1].state = Tile::up;
                        body[0].y -= 64;
                        break;
                    case Tile::down:
                        body[body.size() - 1].x = body[0].x;
                        body[body.size() - 1].y = body[0].y;
                        body[body.size() - 1].state = Tile::down;
                        body[0].y += 64;
                        break;
                    }
                    isMoved = true;
                    checkCollisionWithBody();
                }
            }
            if (plNumber == "1") {
                setPlayerCoordinateForView(body[0].x, body[0].y, speed);
            } else if (plNumber == "2") {
                if (name == "Player1")
                    setPlayer1CoordinateForView(body[0].x, body[0].y, speed);
                else if (name == "Player2")
                    setPlayer2CoordinateForView(body[0].x, body[0].y, speed);
            }
        }
        return isMoved;
    }
    void setBodySprite(unsigned int i)
    {
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
    }
};
#endif
