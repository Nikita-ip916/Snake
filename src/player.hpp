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
    float x, y, dx, dy, speed, moveTimer;
    int w, h, size, score;
    bool life;
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
        score = moveTimer = 0;
        speed = 1;
        dx = dy = 0;
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
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            body[0].state = Tile::left;
        } else if (Keyboard::isKeyPressed(Keyboard::Right)) {
            body[0].state = Tile::right;
        } else if (Keyboard::isKeyPressed(Keyboard::Up)) {
            body[0].state = Tile::up;
        } else if (Keyboard::isKeyPressed(Keyboard::Down)) {
            body[0].state = Tile::down;
        }
    }
    void checkCollisionWithMap()
    {
        for (int i = body[0].y / 64; i < (body[0].y + h) / 64; i++) {
            for (int j = body[0].x / 64; j < (body[0].x + w) / 64; j++) {
                if (TileMap[i][j] == '0') {
                    life = false;
                } else if (TileMap[i][j] == '+') {
                    speed *= 2;
                    // bonusTimer = true;
                    TileMap[i][j] = ' ';
                } else if (TileMap[i][j] == '-') {
                    speed *= 0.5;
                    TileMap[i][j] = ' ';
                }
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
                switch (body[0].state) {
                case Tile::left:
                    body[1].x = body[0].x;
                    body[1].y = body[0].y;
                    body[1].state = Tile::left;
                    body[0].x -= 64;
                    break;
                case Tile::right:
                    body[1].x = body[0].x;
                    body[1].y = body[0].y;
                    body[1].state = Tile::right;
                    body[0].x += 64;
                    break;
                case Tile::up:
                    body[1].x = body[0].x;
                    body[1].y = body[0].y;
                    body[1].state = Tile::up;
                    body[0].y -= 64;
                    break;
                case Tile::down:
                    body[1].x = body[0].x;
                    body[1].y = body[0].y;
                    body[1].state = Tile::down;
                    body[0].y += 64;
                    break;
                }
                checkCollisionWithMap();
                isMoved = true;
            }
            setPlayerCoordinateForView(body[0].x, body[0].y);
        } else {
            sprite.setColor(Color::Red);
        }
        return isMoved;
    }
};
#endif
