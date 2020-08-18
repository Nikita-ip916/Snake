#include "SFML/Graphics.hpp"
#include "player.hpp"
#include "view.hpp"
#include <iomanip>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace sf;

int main()
{
    const int defaultTextSize = 50;
    string plNumber = "1";
    cout << "Введите кол-во игроков(1 или 2):\n";
    do {
        cin >> plNumber;
    } while (plNumber != "1" && plNumber != "2" && plNumber != "one"
             && plNumber != "two" && plNumber != "один" && plNumber != "два");

    if (plNumber == "one" || plNumber == "один") {
        plNumber = "1";
    } else if (plNumber == "two" || plNumber == "два") {
        plNumber = "2";
    }
    RenderWindow window(VideoMode(1920, 1080), "Snake 2020", Style::Fullscreen);
    window.setVerticalSyncEnabled(true);
    changeView(plNumber);

    Image tiles;
    tiles.loadFromFile("images/tiles.png");
    Texture mapTexture;
    mapTexture.loadFromImage(tiles);
    Sprite map;
    map.setTexture(mapTexture);

    Font font;
    font.loadFromFile("images/PostModern.ttf");
    Text bonusText("", font, defaultTextSize),
            scoreText("", font, defaultTextSize),
            gameOver(L"    Игра\nокончена", font, defaultTextSize * 4),
            gameWon(L"      Вы\nпобедили", font, defaultTextSize * 4);
    bonusText.setFillColor(Color(77, 64, 37));
    scoreText.setFillColor(Color(77, 64, 37));
    gameOver.setFillColor(Color::Red);
    gameOver.setStyle(Text::Bold);
    gameWon.setFillColor(Color::Cyan);
    gameWon.setStyle(Text::Bold);
    while (window.isOpen()) {
        Player p1(tiles, plNumber, "Player1", 512, 768, 64, 64);
        Player p2(tiles, plNumber, "Player2", 512, 1280, 64, 64);

        Clock clockMove[3], clockBonus[3];
        bool bonusClock[3];
        bonusClock[1] = bonusClock[2] = false;
        clearMap();
        p1.randomMapGenerate('+');
        p1.randomMapGenerate('a');

        while (window.isOpen()) {
            ostringstream bonus[3], score[3];
            float currentMoveDelay[3];
            currentMoveDelay[1]
                    = clockMove[1].getElapsedTime().asMilliseconds();
            currentMoveDelay[2]
                    = clockMove[2].getElapsedTime().asMilliseconds();
            int bonusTime[3], currentBonusTime[3];
            Event event;
            while (window.pollEvent(event))
                if (event.type == Event::Closed)
                    window.close();
            if (Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();
            if (Keyboard::isKeyPressed(Keyboard::R))
                break;
            score[1] << setfill('0') << setw(2) << p1.score;
            p1.oppositeBody.clear();
            for (unsigned i = 0; i < p2.body.size(); i++) {
                Player::Tile tile;
                tile.x = p2.body[i].x;
                tile.y = p2.body[i].y;
                p1.oppositeBody.push_back(tile);
            }
            if (p1.update(currentMoveDelay[1])) {
                p1.randomMapGenerate('-');
                clockMove[1].restart();
            }
            if (plNumber == "2") {
                score[2] << setfill('0') << setw(2) << p2.score;
                p2.oppositeBody.clear();
                for (unsigned i = 0; i < p1.body.size(); i++) {
                    Player::Tile tile;
                    tile.x = p1.body[i].x;
                    tile.y = p1.body[i].y;
                    p2.oppositeBody.push_back(tile);
                }
                if (p1.life)
                    if (p2.update(currentMoveDelay[2])) {
                        p2.randomMapGenerate('-');
                        clockMove[2].restart();
                    }
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
                if (bonusTime[i] > 0)
                    bonus[i] << setfill('0') << setw(2)
                             << 10 - currentBonusTime[i];
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

            window.setView(view);
            for (int i = 0; i < HEIGHT_MAP; i++)
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
            if (plNumber == "2")
                for (unsigned int i = 0; i < p2.body.size(); i++) {
                    p2.setBodySprite(i);
                    window.draw(p2.sprite);
                    p2.sprite.setRotation(0);
                }
            for (unsigned int i = 0; i < p1.body.size(); i++) {
                p1.setBodySprite(i);
                window.draw(p1.sprite);
                p1.sprite.setRotation(0);
            }
            if (plNumber == "2") {
                scoreText.setString(L"Яблок собрано: " + score[1].str());
                score[1].str("");
                scoreText.setPosition(
                        view.getCenter().x - 406, view.getCenter().y - 470);
                window.draw(scoreText);
                if (!p2.life && !p2.winner) {
                    p2.sprite.setColor(Color::Black);
                    p1.winner = true;
                    p1.life = false;
                }
                if (p1.winner) {
                    gameWon.setPosition(
                            view.getCenter().x - 440, view.getCenter().y - 256);
                    window.draw(gameWon);
                } else if (p1.life) {
                    bonusText.setString(L"Действие бонуса: " + bonus[1].str());
                    bonus[1].str("");
                    bonusText.setPosition(
                            view.getCenter().x - 406, view.getCenter().y - 406);
                    window.draw(bonusText);
                } else {
                    gameOver.setPosition(
                            view.getCenter().x - 440, view.getCenter().y - 256);
                    window.draw(gameOver);
                }
            } else if (plNumber == "1") {
                scoreText.setString(L"Яблок собрано: " + score[1].str());
                score[1].str("");
                scoreText.setPosition(
                        view.getCenter().x - 890, view.getCenter().y - 470);
                window.draw(scoreText);
                if (p1.life) {
                    bonusText.setString(L"Действие бонуса: " + bonus[1].str());
                    bonus[1].str("");
                    bonusText.setPosition(
                            view.getCenter().x - 890, view.getCenter().y - 406);
                    window.draw(bonusText);
                } else {
                    p1.sprite.setColor(Color::Black);
                    gameOver.setPosition(
                            view.getCenter().x - 450, view.getCenter().y - 150);
                    window.draw(gameOver);
                }
            }

            if (plNumber == "2") {
                window.setView(viewP2);
                for (int i = 0; i < HEIGHT_MAP; i++)
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
                for (unsigned int i = 0; i < p1.body.size(); i++) {
                    p1.setBodySprite(i);
                    window.draw(p1.sprite);
                    p1.sprite.setRotation(0);
                }
                for (unsigned int i = 0; i < p2.body.size(); i++) {
                    p2.setBodySprite(i);
                    window.draw(p2.sprite);
                    p2.sprite.setRotation(0);
                }
                scoreText.setString(L"Яблок собрано: " + score[2].str());
                score[2].str("");
                scoreText.setPosition(
                        viewP2.getCenter().x - 415, viewP2.getCenter().y - 470);
                window.draw(scoreText);
                if (!p1.life && !p1.winner) {
                    p1.sprite.setColor(Color::Black);
                    p2.winner = true;
                    p2.life = false;
                }
                if (p2.winner) {
                    gameWon.setPosition(
                            viewP2.getCenter().x - 440,
                            viewP2.getCenter().y - 256);
                    window.draw(gameWon);
                } else if (p2.life) {
                    bonusText.setString(L"Действие бонуса: " + bonus[2].str());
                    bonus[2].str("");
                    bonusText.setPosition(
                            viewP2.getCenter().x - 415,
                            viewP2.getCenter().y - 406);
                    window.draw(bonusText);
                } else {
                    gameOver.setPosition(
                            viewP2.getCenter().x - 440,
                            viewP2.getCenter().y - 256);
                    window.draw(gameOver);
                }
            }
            window.display();
        }
    }
    return 0;
}
