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

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

RenderWindow window;
Font font;
const int defaultTextSize = 50;

void display(int screenW, int screenH, string plNumber)
{
    window.create(VideoMode(screenW, screenH), "Snake 2020", Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    Image tiles;
    tiles.loadFromFile("images/tiles.png");
    font.loadFromFile("images/PostModern.ttf");
    Text gameOver(L"    Игра\nокончена", font, defaultTextSize * 4),
            gameWon(L"      Вы\nпобедили", font, defaultTextSize * 4);
    gameOver.setFillColor(Color::Red);
    gameOver.setStyle(Text::Bold);
    gameWon.setFillColor(Color::Cyan);
    gameWon.setStyle(Text::Bold);
    while (window.isOpen()) {
        Map map;
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();
        if (Keyboard::isKeyPressed(Keyboard::R))
            break;
        if (plNumber == "1") {
            Player player(tiles, screenW / 4 / 64 * 64, screenW / 2 / 64 * 64);
            map.clearMap();
            map.randomMapGenerate("+", player.getBody());
            map.randomMapGenerate("a", player.getBody());
        } else if (plNumber == "2") {
            Player** player = new Player*[2];
            player[0] = new Player1(
                    tiles, screenW / 4 / 64 * 64, screenW / 2 / 64 * 64);
            player[1] = new Player2(
                    tiles, screenW / 4 * 3 / 64 * 64, screenW / 2 / 64 * 64);
            map.clearMap();
            map.randomMapGenerate(
                    "+", player[0].getBody(), player[1].getBody());
            map.randomMapGenerate(
                    "a", player[0].getBody(), player[1].getBody());
        }
    }
}
///////////////////////////////////////////////////////////////// Bonus Updates
void setParametrs(Map& map, Player& player)
{
    if (player.update(map)) {
        map.randomMapGenerate('-', player.getBody());
        player.getClockMove.restart();
    }
}
void setParametrs(Map& map, Player** player)
{
    if (player[0]->update(map, player[1]->getBody())) {
        map.randomMapGenerate('-', player[0]->getBody(), player[1]->getBody());
        player[0]->getClockMove.restart();
    }
    if (player[0]->getLife())
        if (player[1]->update(map, player[0]->getBody())) {
            map.randomMapGenerate(
                    '-', player[1]->getBody(), player[0]->getBody());
            player[1]->getClockMove.restart();
        }
}
#endif
