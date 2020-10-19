#include <SFML/Graphics.hpp>
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

void Player::growth()
{
    Tile tile;
    tile.name = "Body";
    tile.x = body[0].x;
    tile.y = body[0].y;
    tile.state = body[0].state;
    body.push_back(tile);
}

void Player::checkCollisionWithMap(Bonus booster, Bonus slower, Bonus apple)
{
    eaten = false;
    int i = body[0].y / 64;
    int j = body[0].x / 64;
    if (TileMap[i][j] == '0') {
        life = false;
    } else if (TileMap[i][j] == '+') {
        speed = 2;
        bonusTimer = true;
        TileMap[i][j] = ' ';
        booster.randomMapGenerate(body);
    } else if (TileMap[i][j] == '-') {
        speed = 0.5;
        bonusTimer = true;
        TileMap[i][j] = ' ';
        clearSlowers();
    } else if (TileMap[i][j] == 'a') {
        score++;
        eaten = true;
        TileMap[i][j] = ' ';
        growth();
        booster.randomMapGenerate(body);
        apple.randomMapGenerate(body);
    }
}

void Player::checkCollisionWithMap(
        Bonus booster, Bonus slower, Bonus apple, vector<Tile>& oppositeBody)
{
    eaten = false;
    int i = body[0].y / 64;
    int j = body[0].x / 64;
    if (TileMap[i][j] == '0') {
        life = false;
    } else if (TileMap[i][j] == '+') {
        speed = 2;
        bonusTimer = true;
        TileMap[i][j] = ' ';
        booster.randomMapGenerate(body, oppositeBody);
    } else if (TileMap[i][j] == '-') {
        speed = 0.5;
        bonusTimer = true;
        TileMap[i][j] = ' ';
        clearSlowers();
    } else if (TileMap[i][j] == 'a') {
        score++;
        eaten = true;
        TileMap[i][j] = ' ';
        growth();
        booster.randomMapGenerate(body, oppositeBody);
        apple.randomMapGenerate(body, oppositeBody);
    }
}

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

bool Player::update(
        float currentMoveDelay, Bonus& booster, Bonus& slower, Bonus& apple)
{
    if (life)
        control();
    if (life && currentMoveDelay * speed > MOVE_DELAY) {
        checkCollisionWithMap(booster, slower, apple);
        checkCollisionWithBody();
    }
    bool isMoved = midUpdate(currentMoveDelay);
    setPlayerCoordinateForView();
    return isMoved;
}

bool Player::update(
        float currentMoveDelay,
        Bonus& booster,
        Bonus& slower,
        Bonus& apple,
        vector<Tile>& oppositeBody)
{
    if (life)
        control();
    if (life && currentMoveDelay * speed > MOVE_DELAY) {
        checkCollisionWithMap(booster, slower, apple, oppositeBody);
        checkCollisionWithBody(oppositeBody);
    }
    bool isMoved = midUpdate(currentMoveDelay);
    setPlayerCoordinateForView();
    return isMoved;
}

void Player::setBodySprite(insigned int i)
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

void Player::setTemps(int& tempX, int& tempY)
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

    setTemps(tempX, tempY);

    view.setCenter(
            view.getCenter().x
                    + (tempX - view.getCenter().x) / (halfW / 20 / speed),
            view.getCenter().y
                    + (tempY - view.getCenter().y) / (halfH / 20 / speed));
}

void Player1::setPlayerCoordinateForView(int screenW, int screenH)
{
    float tempX = body[0].x, tempY = body[0].y;
    int halfW = screenW / 4 - 6, halfH = screenH / 2;

    setTemps(tempX, tempY);

    view.setCenter(
            view.getCenter().x
                    + (tempX - view.getCenter().x) / (halfW / 20 / speed),
            view.getCenter().y
                    + (tempY - view.getCenter().y) / (halfH / 20 / speed));
}

void Player2::setPlayerCoordinateForView(int screenW, int screenH)
{
    float tempX = body[0].x, tempY = body[0].y;
    int halfW = screenW / 4 - 6, halfH = screenH / 2;

    setTemps(tempX, tempY);

    viewP2.setCenter(
            viewP2.getCenter().x
                    + (tempX - viewP2.getCenter().x) / (halfW / 20 / speed),
            viewP2.getCenter().y
                    + (tempY - viewP2.getCenter().y) / (halfH / 20 / speed));
}

void Bonus::randomMapGenerate(
        int countBonus = 1,
        vector<Tile>& playerBody,
        vector<Tile>& oppositeBody)
{
    int randomElementX = 0;
    int randomElementY = 0;
    bool generate;
    srand(time(0));
    while (countBonus > 0) {
        generate = true;
        randomElementX = 1 + rand() % (WIDTH_MAP - 1);
        randomElementY = 1 + rand() % (HEIGHT_MAP - 1);
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

#endif
