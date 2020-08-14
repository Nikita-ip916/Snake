#include "SFML/Graphics.hpp"
#include "player.hpp"
#include "view.hpp"
#include <iomanip>
#include <iostream>
//#include <locale>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
//#include <string>

using namespace std;
using namespace sf;

int main()
{
    // setlocale(LC_CTYPE, "rus");
    const int defaultTextSize = 50;
    RenderWindow window(VideoMode(1920, 1080), "Snake 2020", Style::Fullscreen);
    window.setVerticalSyncEnabled(true);
    view.reset(FloatRect(0, 0, 1920, 1080));
    viewP2.reset(FloatRect(0, 0, 0, 0));

    Image mapImage;
    mapImage.loadFromFile("images/tiles.png");
    Texture mapTexture;
    mapTexture.loadFromImage(mapImage);
    Sprite map;
    map.setTexture(mapTexture);

    Image heroImage;
    heroImage.loadFromFile("images/tiles.png");
    Font font;
    font.loadFromFile("images/PostModern.ttf");
    Text bonusText("", font, defaultTextSize);
    Text scoreText("", font, defaultTextSize);
    Text gameOver(L"    Игра\nокончена", font, defaultTextSize * 4);

    bonusText.setFillColor(Color(77, 64, 37));
    scoreText.setFillColor(Color(77, 64, 37));
    gameOver.setFillColor(Color::Red);
    gameOver.setStyle(Text::Bold);

    Player p1(heroImage, "Player1", 256, 128, 64, 64);
    Player p2(heroImage, "Player2", 512, 256, 64, 64);

    Clock clockMove[3], clockBonus[3];
    bool bonusClock[3];
    bonusClock[1] = bonusClock[2] = false;

    p1.randomMapGenerate('+');
    p1.randomMapGenerate('a');

    while (window.isOpen()) {
        ostringstream bonus[3], score[3];
        float currentMoveDelay[3];
        currentMoveDelay[1] = clockMove[1].getElapsedTime().asMilliseconds();
        currentMoveDelay[2] = clockMove[2].getElapsedTime().asMilliseconds();
        int bonusTime[3], currentBonusTime[3];
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }
        score[1] << setfill('0') << setw(2) << p1.score;
        score[2] << setfill('0') << setw(2) << p2.score;
        if (p1.update(currentMoveDelay[1])) {
            p1.randomMapGenerate('-');
            clockMove[1].restart();
        }
        if (p2.update(currentMoveDelay[2])) {
            p2.randomMapGenerate('-');
            clockMove[2].restart();
        }
        if (p1.bonusTimer) {
            bonusTime[1] = 10;
            bonusClock[1] = true;
            clockBonus[1].restart();
            p1.bonusTimer = false;
        }
        if (p2.bonusTimer) {
            bonusTime[2] = 10;
            bonusClock[2] = true;
            clockBonus[2].restart();
            p2.bonusTimer = false;
        }
        for (int i = 1; i <= 2; i++) {
            if (bonusClock[i]) {
                currentBonusTime[i]
                        = clockBonus[i].getElapsedTime().asSeconds();
            } else {
                bonusTime[i] = 0;
            }
            if (bonusTime[i] > 0) {
                bonus[i] << setfill('0') << setw(2) << 10 - currentBonusTime[i];
            }
        }
        if (currentBonusTime[1] >= bonusTime[1]) {
            p1.speed = 1;
            bonusTime[1] = 0;
            bonusClock[1] = false;
        }
        if (currentBonusTime[2] >= bonusTime[2]) {
            p2.speed = 1;
            bonusTime[2] = 0;
            bonusClock[2] = false;
        }
        window.clear(Color(77, 64, 37));
        changeView();

        window.setView(view);
        for (int i = 0; i < HEIGHT_MAP; i++) {
            for (int j = 0; j < WIDTH_MAP; j++) {
                if (TileMap[i][j] == ' ') {
                    map.setTextureRect(IntRect(320, 0, 64, 64));
                }
                if (TileMap[i][j] == 'a') {
                    map.setTextureRect(IntRect(448, 0, 64, 64));
                }
                if (TileMap[i][j] == '0') {
                    map.setTextureRect(IntRect(384, 0, 64, 64));
                }
                if (TileMap[i][j] == '+') {
                    map.setTextureRect(IntRect(576, 0, 64, 64));
                }
                if (TileMap[i][j] == '-') {
                    map.setTextureRect(IntRect(512, 0, 64, 64));
                }
                map.setPosition(j * 64, i * 64);
                window.draw(map);
            }
        }

        for (unsigned int i = 0; i < p1.body.size(); i++) {
            if (p1.body[i].name == "Head") {
                p1.sprite.setTextureRect(IntRect(0, 0, 64, 64));
            } else if (p1.body[i].name == "Body") {
                if (p1.body[i].state == p1.body[i - 1].state) {
                    p1.sprite.setTextureRect(IntRect(64, 0, 64, 64));
                } else if (
                        (p1.body[i].state == Player::Tile::left
                         && p1.body[i - 1].state == Player::Tile::up)
                        || (p1.body[i].state == Player::Tile::up
                            && p1.body[i - 1].state == Player::Tile::right)
                        || (p1.body[i].state == Player::Tile::right
                            && p1.body[i - 1].state == Player::Tile::down)
                        || (p1.body[i].state == Player::Tile::down
                            && p1.body[i - 1].state == Player::Tile::left)) {
                    p1.sprite.setTextureRect(IntRect(192, 0, 64, 64));
                } else {
                    p1.sprite.setTextureRect(IntRect(128, 0, 64, 64));
                }
            } else if (p1.body[i].name == "Tail") {
                p1.sprite.setTextureRect(IntRect(256, 0, 64, 64));
            }
            switch (p1.body[i].state) {
            case Player::Tile::left:
                p1.sprite.setRotation(180);
                break;
            case Player::Tile::right:
                p1.sprite.setRotation(0);
                break;
            case Player::Tile::up:
                p1.sprite.setRotation(270);
                break;
            case Player::Tile::down:
                p1.sprite.setRotation(90);
                break;
            }
            p1.sprite.setPosition(
                    p1.body[i].x + p1.w / 2, p1.body[i].y + p1.h / 2);
            window.draw(p1.sprite);
            p1.sprite.setRotation(0);
        }
        for (unsigned int i = 0; i < p2.body.size(); i++) {
            if (p2.body[i].name == "Head") {
                p2.sprite.setTextureRect(IntRect(0, 0, 64, 64));
            } else if (p2.body[i].name == "Body") {
                if (p2.body[i].state == p2.body[i - 1].state) {
                    p2.sprite.setTextureRect(IntRect(64, 0, 64, 64));
                } else if (
                        (p2.body[i].state == Player::Tile::left
                         && p2.body[i - 1].state == Player::Tile::up)
                        || (p2.body[i].state == Player::Tile::up
                            && p2.body[i - 1].state == Player::Tile::right)
                        || (p2.body[i].state == Player::Tile::right
                            && p2.body[i - 1].state == Player::Tile::down)
                        || (p2.body[i].state == Player::Tile::down
                            && p2.body[i - 1].state == Player::Tile::left)) {
                    p2.sprite.setTextureRect(IntRect(192, 0, 64, 64));
                } else {
                    p2.sprite.setTextureRect(IntRect(128, 0, 64, 64));
                }
            } else if (p2.body[i].name == "Tail") {
                p2.sprite.setTextureRect(IntRect(256, 0, 64, 64));
            }
            switch (p2.body[i].state) {
            case Player::Tile::left:
                p2.sprite.setRotation(180);
                break;
            case Player::Tile::right:
                p2.sprite.setRotation(0);
                break;
            case Player::Tile::up:
                p2.sprite.setRotation(270);
                break;
            case Player::Tile::down:
                p2.sprite.setRotation(90);
                break;
            }
            p2.sprite.setPosition(
                    p2.body[i].x + p2.w / 2, p2.body[i].y + p2.h / 2);
            window.draw(p2.sprite);
            p2.sprite.setRotation(0);
        }

        if (p1.life) {
            bonusText.setString(L"Действие бонуса: " + bonus[1].str());
            bonus[1].str("");
            bonusText.setPosition(
                    view.getCenter().x - 406, view.getCenter().y - 406);
            window.draw(bonusText);
            scoreText.setString(L"Яблок собрано: " + score[1].str());
            score[1].str("");
            scoreText.setPosition(
                    view.getCenter().x - 406, view.getCenter().y - 470);
            window.draw(scoreText);
        } else {
            gameOver.setPosition(
                    view.getCenter().x - 440, view.getCenter().y - 256);
            window.draw(gameOver);
        }

        window.setView(viewP2);
        for (int i = 0; i < HEIGHT_MAP; i++) {
            for (int j = 0; j < WIDTH_MAP; j++) {
                if (TileMap[i][j] == ' ') {
                    map.setTextureRect(IntRect(320, 0, 64, 64));
                }
                if (TileMap[i][j] == 'a') {
                    map.setTextureRect(IntRect(448, 0, 64, 64));
                }
                if (TileMap[i][j] == '0') {
                    map.setTextureRect(IntRect(384, 0, 64, 64));
                }
                if (TileMap[i][j] == '+') {
                    map.setTextureRect(IntRect(576, 0, 64, 64));
                }
                if (TileMap[i][j] == '-') {
                    map.setTextureRect(IntRect(512, 0, 64, 64));
                }
                map.setPosition(j * 64, i * 64);
                window.draw(map);
            }
        }
        for (unsigned int i = 0; i < p1.body.size(); i++) {
            if (p1.body[i].name == "Head") {
                p1.sprite.setTextureRect(IntRect(0, 0, 64, 64));
            } else if (p1.body[i].name == "Body") {
                if (p1.body[i].state == p1.body[i - 1].state) {
                    p1.sprite.setTextureRect(IntRect(64, 0, 64, 64));
                } else if (
                        (p1.body[i].state == Player::Tile::left
                         && p1.body[i - 1].state == Player::Tile::up)
                        || (p1.body[i].state == Player::Tile::up
                            && p1.body[i - 1].state == Player::Tile::right)
                        || (p1.body[i].state == Player::Tile::right
                            && p1.body[i - 1].state == Player::Tile::down)
                        || (p1.body[i].state == Player::Tile::down
                            && p1.body[i - 1].state == Player::Tile::left)) {
                    p1.sprite.setTextureRect(IntRect(192, 0, 64, 64));
                } else {
                    p1.sprite.setTextureRect(IntRect(128, 0, 64, 64));
                }
            } else if (p1.body[i].name == "Tail") {
                p1.sprite.setTextureRect(IntRect(256, 0, 64, 64));
            }
            switch (p1.body[i].state) {
            case Player::Tile::left:
                p1.sprite.setRotation(180);
                break;
            case Player::Tile::right:
                p1.sprite.setRotation(0);
                break;
            case Player::Tile::up:
                p1.sprite.setRotation(270);
                break;
            case Player::Tile::down:
                p1.sprite.setRotation(90);
                break;
            }
            p1.sprite.setPosition(
                    p1.body[i].x + p1.w / 2, p1.body[i].y + p1.h / 2);
            window.draw(p1.sprite);
            p1.sprite.setRotation(0);
        }
        for (unsigned int i = 0; i < p2.body.size(); i++) {
            if (p2.body[i].name == "Head") {
                p2.sprite.setTextureRect(IntRect(0, 0, 64, 64));
            } else if (p2.body[i].name == "Body") {
                if (p2.body[i].state == p2.body[i - 1].state) {
                    p2.sprite.setTextureRect(IntRect(64, 0, 64, 64));
                } else if (
                        (p2.body[i].state == Player::Tile::left
                         && p2.body[i - 1].state == Player::Tile::up)
                        || (p2.body[i].state == Player::Tile::up
                            && p2.body[i - 1].state == Player::Tile::right)
                        || (p2.body[i].state == Player::Tile::right
                            && p2.body[i - 1].state == Player::Tile::down)
                        || (p2.body[i].state == Player::Tile::down
                            && p2.body[i - 1].state == Player::Tile::left)) {
                    p2.sprite.setTextureRect(IntRect(192, 0, 64, 64));
                } else {
                    p2.sprite.setTextureRect(IntRect(128, 0, 64, 64));
                }
            } else if (p2.body[i].name == "Tail") {
                p2.sprite.setTextureRect(IntRect(256, 0, 64, 64));
            }
            switch (p2.body[i].state) {
            case Player::Tile::left:
                p2.sprite.setRotation(180);
                break;
            case Player::Tile::right:
                p2.sprite.setRotation(0);
                break;
            case Player::Tile::up:
                p2.sprite.setRotation(270);
                break;
            case Player::Tile::down:
                p2.sprite.setRotation(90);
                break;
            }
            p2.sprite.setPosition(
                    p2.body[i].x + p2.w / 2, p2.body[i].y + p2.h / 2);
            window.draw(p2.sprite);
            p2.sprite.setRotation(0);
        }

        if (p2.life) {
            bonusText.setString(L"Действие бонуса: " + bonus[2].str());
            bonus[2].str("");
            bonusText.setPosition(
                    viewP2.getCenter().x - 415, viewP2.getCenter().y - 406);
            window.draw(bonusText);
            scoreText.setString(L"Яблок собрано: " + score[2].str());
            score[2].str("");
            scoreText.setPosition(
                    viewP2.getCenter().x - 415, viewP2.getCenter().y - 470);
            window.draw(scoreText);
        } else {
            gameOver.setPosition(
                    viewP2.getCenter().x - 440, viewP2.getCenter().y - 256);
            window.draw(gameOver);
        }
        window.display();
    }
    return 0;
}
