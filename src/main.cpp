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
    view.reset(FloatRect(0, 0, 1920, 1080));

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

    Player p1(heroImage, "Player1", 256, 128, 64, 64);

    Clock clockMove, clockBonus;
    bool bonusClock = false;

    p1.randomMapGenerate('+');
    p1.randomMapGenerate('-');
    p1.randomMapGenerate('a');

    while (window.isOpen()) {
        ostringstream bonus, score;
        float currentMoveDelay = clockMove.getElapsedTime().asMilliseconds();
        int bonusTime, currentBonusTime;
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        score << "Score: " << setfill('0') << setw(2) << p1.score;
        if (p1.update(currentMoveDelay)) {
            clockMove.restart();
        }
        if (p1.bonusTimer) {
            bonusTime = 10;
            bonusClock = true;
            clockBonus.restart();
            p1.bonusTimer = false;
        }
        if (bonusClock) {
            currentBonusTime = clockBonus.getElapsedTime().asSeconds();
        } else {
            bonusTime = 0;
        }
        if (bonusTime > 0) {
            bonus << "Bonus remain: " << setfill('0') << setw(2)
                  << 10 - currentBonusTime;
        }
        if (currentBonusTime >= bonusTime) {
            p1.speed = 1;
            bonusTime = 0;
            bonusClock = false;
        }
        window.setView(view);
        window.clear();
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
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }
        if (p1.life) {
            bonusText.setString(bonus.str());
            bonus.str("");
            bonusText.setPosition(
                    view.getCenter().x - 634, view.getCenter().y - 470);
            window.draw(bonusText);
            scoreText.setString(score.str());
            score.str("");
            scoreText.setPosition(
                    view.getCenter().x - 890, view.getCenter().y - 470);
            window.draw(scoreText);
        }
        window.display();
    }
    return 0;
}
