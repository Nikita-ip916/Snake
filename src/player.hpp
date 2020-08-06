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
    vector<Tile> body;
    Tile tile;
    float x, y, dx, dy, speed;
    int w, h, score;
    bool life, bonusTimer;
    Texture texture;
    Sprite sprite;
    string name;
    Player(Image& image, string Name, float X, float Y, float W, float H)
    {
        x = tile.x = X;
        y = tile.y = Y;
        w = W;
        h = H;
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
        dx = dy = 0;
        bonusTimer = false;
        life = true;
        image.createMaskFromColor(Color(255, 255, 255));
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setOrigin(w / 2, h / 2);
        if (name == "Player2") {
            sprite.setColor(Color::Red);
        }
    }
    void control()
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
    bool checkCollisionWithMap()
    {
        bool eaten = false;
        for (int i = body[0].y / 64; i < (body[0].y + h) / 64; i++) {
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
                    randomMapGenerate('-');
                } else if (TileMap[i][j] == 'a') {
                    score++;
                    eaten = true;
                    TileMap[i][j] = ' ';
                    growth();
                    randomMapGenerate('a');
                }
            }
        }
        return eaten;
    }
    void checkCollisionWithBody()
    {
        for (unsigned int i = 1; i < body.size(); i++) {
            if (body[0].x == body[i].x && body[0].y == body[i].y) {
                life = false;
            }
        }
    }
    bool update(float currentMoveDelay)
    {
        bool isMoved = false;
        float moveDelay = 700;
        if (life) {
            control();
            if (currentMoveDelay * speed > moveDelay) {
                if (!checkCollisionWithMap()) {
                    for (unsigned int i = 1; i < body.size() - 1; i++) {
                        body[i].x = body[i + 1].x;
                        body[i].y = body[i + 1].y;
                        body[i].state = body[i + 1].state;
                    }
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
            setPlayerCoordinateForView(body[0].x, body[0].y);
        } else {
            sprite.setColor(Color::Red);
        }
        return isMoved;
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
            for (unsigned int i = 1; i < body.size(); i++) {
                if (body[i].x == randomElementX * 64
                    && body[i].y == randomElementY * 64) {
                    generate = false;
                }
            }
            if (TileMap[randomElementY][randomElementX] == ' ' && generate) {
                TileMap[randomElementY][randomElementX] = bonus;
                countBonus--;
            }
        }
    }
};
#endif
