#include "SFML/Graphics.hpp"
#include "player.hpp"
#include <iomanip>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace sf;

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

void setParametrs(Map&, Player&, int, int);
void setParametrs(Map&, Player**, int, int);
void draw(int, int, Player**, int);

void display(int screenW, int screenH, string plNumber)
{
    window.create(VideoMode(screenW, screenH), "Snake 2020", Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    Image tiles;
    tiles.loadFromFile("images/tiles.png");
    font.loadFromFile("images/PostModern.ttf");

    gameOver.setFont(font);
    gameOver.setString(L"    Игра\nокончена");
    gameOver.setCharacterSize(DEFAULT_TEXT_SIZE * 4);
    gameOver.setFillColor(Color::Red);
    gameOver.setStyle(Text::Bold);

    gameWon.setFont(font);
    gameWon.setString(L"      Вы\nпобедили");
    gameWon.setCharacterSize(DEFAULT_TEXT_SIZE * 4);
    gameWon.setFillColor(Color::Cyan);
    gameWon.setStyle(Text::Bold);

    while (window.isOpen()) {
        Map map(tiles);
        if (plNumber == "1") {
            Player player(tiles, screenW / 4 / 64 * 64, screenW / 2 / 64 * 64);
            player.changeView(screenW, screenH);
            map.clearMap();
            map.randomMapGenerate('+', player.getBody());
            map.randomMapGenerate('a', player.getBody());
            while (window.isOpen()) {
                Event event;
                while (window.pollEvent(event))
                    if (event.type == Event::Closed)
                        window.close();
                if (Keyboard::isKeyPressed(Keyboard::Escape))
                    window.close();
                if (Keyboard::isKeyPressed(Keyboard::R))
                    break;

                setParametrs(map, player, screenW, screenH);
                window.setView(view);
                map.draw();
                player.draw();
                player.draw(screenW, screenH);
                window.display();
            }
        } else if (plNumber == "2") {
            Player** player = new Player*[2];
            player[0] = new Player1(
                    tiles, screenW / 4 / 64 * 64, screenW / 2 / 64 * 64);
            player[1] = new Player2(
                    tiles, screenW / 4 * 3 / 64 * 64, screenW / 2 / 64 * 64);
            player[0]->changeView(screenW, screenH);
            player[1]->changeView(screenW, screenH);
            map.clearMap();
            map.randomMapGenerate(
                    '+', player[0]->getBody(), player[1]->getBody());
            map.randomMapGenerate(
                    'a', player[0]->getBody(), player[1]->getBody());
            while (window.isOpen()) {
                Event event;
                while (window.pollEvent(event))
                    if (event.type == Event::Closed)
                        window.close();
                if (Keyboard::isKeyPressed(Keyboard::Escape))
                    window.close();
                if (Keyboard::isKeyPressed(Keyboard::R))
                    break;

                setParametrs(map, player, screenW, screenH);

                window.setView(view);
                map.draw();
                player[1]->draw();
                player[0]->draw();
                draw(screenW, screenH, player, 0);
                //................

                window.setView(viewP2);
                map.draw();
                player[0]->draw();
                player[1]->draw();
                draw(screenW, screenH, player, 1);

                window.display();
            }
        }
    }
}
///////////////////////////////////////////////////////////////// Bonus Updates
void setParametrs(Map& map, Player& player, int screenW, int screenH)
{
    if (player.update(map, screenW, screenH)) {
        map.randomMapGenerate('-', player.getBody());
        player.getClockMove().restart();
    }
    player.setBonusTimer();
    window.clear();
}
void setParametrs(Map& map, Player** player, int screenW, int screenH)
{
    if (player[0]->update(map, player[1]->getBody(), screenW, screenH)) {
        map.randomMapGenerate('-', player[0]->getBody(), player[1]->getBody());
        player[0]->getClockMove().restart();
    }
    if (player[0]->getLife())
        if (player[1]->update(map, player[0]->getBody(), screenW, screenH)) {
            map.randomMapGenerate(
                    '-', player[1]->getBody(), player[0]->getBody());
            player[1]->getClockMove().restart();
        }
    player[0]->setBonusTimer();
    player[1]->setBonusTimer();
    window.clear(Color(77, 64, 37));
}
void draw(int screenW, int screenH, Player** player, int currentPlayer)
{
    screenW = 30;
    screenH = 15;
    Text scoreText("", font, DEFAULT_TEXT_SIZE),
            bonusText("", font, DEFAULT_TEXT_SIZE);
    scoreText.setFillColor(Color(77, 64, 37));
    bonusText.setFillColor(Color(77, 64, 37));

    ostringstream scoreStream, bonusStream;
    scoreStream << setfill('0') << setw(2) << player[currentPlayer]->getScore();

    scoreText.setString(L"Яблок собрано: " + scoreStream.str());
    // scoreStream.str("");
    scoreText.setPosition(view.getCenter().x - 406, view.getCenter().y - 470);
    window.draw(scoreText);
    if (!player[1 - currentPlayer]->getLife()
        && !player[1 - currentPlayer]->getWinner()) {
        player[1 - currentPlayer]->getSprite().setColor(Color::Black);
        bool winner = player[currentPlayer]->getWinner();
        bool life = player[currentPlayer]->getLife();
        winner = true;
        life = false;
    }
    if (player[currentPlayer]->getWinner()) {
        gameWon.setPosition(view.getCenter().x - 440, view.getCenter().y - 256);
        window.draw(gameWon);
    } else if (
            player[currentPlayer]->getLife()
            && player[currentPlayer]->getBonusTime() > 0) {
        bonusStream << setfill('0') << setw(2)
                    << player[currentPlayer]->getBonusLeft();
        bonusText.setString(L"Действие бонуса: " + bonusStream.str());
        // bonusStream.str("");
        bonusText.setPosition(
                view.getCenter().x - 406, view.getCenter().y - 406);
        window.draw(bonusText);
    } else if (!player[currentPlayer]->getLife()) {
        gameOver.setPosition(
                view.getCenter().x - 440, view.getCenter().y - 256);
        window.draw(gameOver);
    }
}
#endif
