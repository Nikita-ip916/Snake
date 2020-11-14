#include "SFML/Graphics.hpp"
#include "functions.hpp"
#include "player.hpp"
#include <iomanip>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;
using namespace sf;

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

void draw(int, int, Player**, int*, int);

void display()
{
    string plNumber;
    int screenW = 1920, screenH = 1080;
    window.create(VideoMode(screenW, screenH), "Snake 2020");
    window.setVerticalSyncEnabled(true);

    Event event;

    Image tiles;
    tiles.loadFromFile("images/tiles.png");
    font.loadFromFile("images/PostModern.ttf");

    int* wins = new int[3];
    wins[0] = wins[1] = 0;
    wins[2] = 3;

    gameOver.setFont(font);
    gameOver.setFillColor(Color::Red);
    gameOver.setStyle(Text::Bold);

    gameWon.setFont(font);
    gameWon.setFillColor(Color::Cyan);
    gameWon.setStyle(Text::Bold);

    Map map(tiles);

    int currentResolution = 3;
    bool fullscreen = false;

    while (window.isOpen()) {
        drawMenu(
                map,
                screenW,
                screenH,
                fullscreen,
                currentResolution,
                wins,
                plNumber);

        if (plNumber == "1") {
            Player player(
                    tiles,
                    screenW / 2 / TILE_SIZE * TILE_SIZE,
                    screenH / 2 / TILE_SIZE * TILE_SIZE);
            player.changeView(screenW, screenH);
            map.clearMap();
            map.randomMapGenerate('+', player.getBody());
            map.randomMapGenerate('a', player.getBody());
            while (window.isOpen()) {
                while (window.pollEvent(event))
                    if (event.type == Event::Closed)
                        window.close();
                if (Keyboard::isKeyPressed(Keyboard::Escape))
                    break;

                setParametrs(map, player, screenW, screenH);

                window.setView(view[0]);
                map.draw();
                player.draw();
                player.draw(screenW, screenH);
                window.display();
            }
        } else if (plNumber == "2") {
            while (window.isOpen()) {
                while (window.pollEvent(event))
                    if (event.type == Event::Closed)
                        window.close();
                if (Keyboard::isKeyPressed(Keyboard::Escape))
                    break;

                Player** player = new Player*[2];
                player[0] = new Player1(
                        tiles,
                        HEIGHT_MAP * TILE_SIZE / 4,
                        HEIGHT_MAP * TILE_SIZE / 4);
                player[1] = new Player2(
                        tiles,
                        HEIGHT_MAP * TILE_SIZE / 4,
                        HEIGHT_MAP * TILE_SIZE / 4 * 3);

                player[0]->changeView(screenW, screenH);
                player[1]->changeView(screenW, screenH);

                map.clearMap();
                map.randomMapGenerate(
                        '+', player[0]->getBody(), player[1]->getBody());
                map.randomMapGenerate(
                        'a', player[0]->getBody(), player[1]->getBody());

                bool plusWins = true;

                while (window.isOpen()) {
                    while (window.pollEvent(event))
                        if (event.type == Event::Closed)
                            window.close();

                    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                        wins[0] = wins[1] = 0;
                        break;
                    } else if (
                            Keyboard::isKeyPressed(Keyboard::Return)
                            && (player[0]->getWinner()
                                || player[1]->getWinner())
                            && wins[0] < wins[2] && wins[1] < wins[2]) {
                        break;
                    }

                    setParametrs(map, player, screenW, screenH);

                    window.setView(view[0]);
                    map.draw();
                    player[1]->draw();
                    player[0]->draw();
                    player[0]->draw(screenW, screenH, player[1], wins, 0);

                    window.setView(view[1]);
                    map.draw();
                    player[0]->draw();
                    player[1]->draw();
                    player[1]->draw(screenW, screenH, player[0], wins, 1);

                    window.display();

                    if (player[0]->getWinner() && plusWins) {
                        sleep(1);
                        wins[0]++;
                        plusWins = false;
                    } else if (player[1]->getWinner() && plusWins) {
                        sleep(1);
                        wins[1]++;
                        plusWins = false;
                    }
                }
                delete[] player;
            }
        }
    }
}
#endif
