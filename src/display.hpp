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

void display()
{
    string plNumber;
    int screenW = 800, screenH = 600;
    window.create(VideoMode(screenW, screenH), "Snake 2020");
    window.setVerticalSyncEnabled(true);

    Image tiles;
    tiles.loadFromFile("images/tiles.png");
    font.loadFromFile("images/PostModern.ttf");

    gameOver.setFont(font);
    gameOver.setString(L"    Игра\nокончена");
    gameOver.setCharacterSize(screenW / 10);
    gameOver.setFillColor(Color::Red);
    gameOver.setStyle(Text::Bold);

    gameWon.setFont(font);
    gameWon.setString(L"      Вы\nпобедили");
    gameWon.setCharacterSize(screenW / 10);
    gameWon.setFillColor(Color::Cyan);
    gameWon.setStyle(Text::Bold);

    while (window.isOpen()) {
        Map map(tiles);
        Text* menuText = new Text[4];
        Text* resolutionText = new Text[6];

        menuText[0].setString(L"* Играть одному  ");
        menuText[1].setString(L"* Играть с другом  ");
        menuText[2].setString(L"* Разрешение экрана");
        menuText[3].setString(L"* Выход            ");

        resolutionText[0].setString(L"(800 x 600) SVGA");
        resolutionText[1].setString(L"(1280 x 720) HD");
        resolutionText[2].setString(L"(1600 x 900) WXGA++");
        resolutionText[3].setString(L"(1920 x 1080) Full HD");
        resolutionText[4].setString(L"Fullscreen    ---");
        resolutionText[5].setString(L"Применить изменения");

        Clock clockMenu;
        bool resolutionMenu = false, fullscreen = false, tempFullscreen = false;
        int activeMenu = 0, activeResolution = 0, chosen = 0;
        int tempScreenW = screenW, tempScreenH = screenH;

        window.setView(window.getDefaultView());

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event))
                if (event.type == Event::Closed)
                    window.close();

            float currentMenuDelay
                    = clockMenu.getElapsedTime().asMilliseconds();

            for (int i = 0; i < 6; i++) {
                if (i < 4) {
                    menuText[i].setFont(font);
                    menuText[i].setCharacterSize(screenW / 30 + TILE_SIZE / 4);
                }
                resolutionText[i].setFont(font);
                resolutionText[i].setCharacterSize(
                        screenW / 30 + TILE_SIZE / 4);
            }

            if (resolutionMenu) {
                if ((Keyboard::isKeyPressed(Keyboard::Up)
                     || Keyboard::isKeyPressed(Keyboard::W))
                    && currentMenuDelay > MOVE_DELAY / 3) {
                    if (activeResolution > 0) {
                        activeResolution--;
                    } else
                        activeResolution = 5;
                    clockMenu.restart();
                } else if (
                        (Keyboard::isKeyPressed(Keyboard::Down)
                         || Keyboard::isKeyPressed(Keyboard::S))
                        && currentMenuDelay > MOVE_DELAY / 3) {
                    if (activeResolution < 5) {
                        activeResolution++;
                    } else
                        activeResolution = 0;
                    clockMenu.restart();
                } else if (
                        Keyboard::isKeyPressed(Keyboard::Return)
                        && currentMenuDelay > MOVE_DELAY / 3) {
                    if (activeResolution < 4) {
                        chosen = activeResolution;
                    }
                    if (activeResolution == 0) {
                        tempScreenW = 800;
                        tempScreenH = 600;
                    } else if (activeResolution == 1) {
                        tempScreenW = 1280;
                        tempScreenH = 720;
                    } else if (activeResolution == 2) {
                        tempScreenW = 1600;
                        tempScreenH = 900;
                    } else if (activeResolution == 3) {
                        tempScreenW = 1920;
                        tempScreenH = 1080;
                    } else if (activeResolution == 4) {
                        tempFullscreen = !tempFullscreen;
                    } else if (activeResolution == 5) {
                        if (screenW != tempScreenW || screenH != tempScreenH
                            || fullscreen != tempFullscreen) {
                            screenW = tempScreenW;
                            screenH = tempScreenH;
                            fullscreen = tempFullscreen;
                            window.close();
                            if (fullscreen) {
                                window.create(
                                        VideoMode(screenW, screenH),
                                        "Snake 2020",
                                        Style::Fullscreen);
                            } else
                                window.create(
                                        VideoMode(screenW, screenH),
                                        "Snake 2020");
                        }
                        resolutionMenu = false;
                    }
                    clockMenu.restart();
                }
            } else {
                if ((Keyboard::isKeyPressed(Keyboard::Up)
                     || Keyboard::isKeyPressed(Keyboard::W))
                    && currentMenuDelay > MOVE_DELAY / 3) {
                    if (activeMenu > 0) {
                        activeMenu--;
                    } else
                        activeMenu = 3;
                    clockMenu.restart();
                } else if (
                        (Keyboard::isKeyPressed(Keyboard::Down)
                         || Keyboard::isKeyPressed(Keyboard::S))
                        && currentMenuDelay > MOVE_DELAY / 3) {
                    if (activeMenu < 3) {
                        activeMenu++;
                    } else
                        activeMenu = 0;
                    clockMenu.restart();
                } else if (
                        Keyboard::isKeyPressed(Keyboard::Return)
                        && currentMenuDelay > MOVE_DELAY / 3) {
                    if (activeMenu == 0) {
                        plNumber = "1";
                        break;
                    } else if (activeMenu == 1) {
                        plNumber = "2";
                        break;
                    } else if (activeMenu == 2) {
                        resolutionMenu = true;
                    } else if (activeMenu == 3) {
                        window.close();
                    }
                    clockMenu.restart();
                }
            }
            map.draw();
            if (resolutionMenu) {
                if (tempFullscreen) {
                    resolutionText[4].setString(L"Fullscreen    +++");
                } else {
                    resolutionText[4].setString(L"Fullscreen    ---");
                }
                for (int i = 0; i < 6; i++) {
                    resolutionText[i].setPosition(
                            screenW / 4, screenH / 6 + i * screenH / 10);
                    if (i == activeResolution) {
                        resolutionText[i].setFillColor(Color::Red);
                        resolutionText[i].setStyle(Text::Bold);
                    } else {
                        resolutionText[i].setFillColor(Color::White);
                        resolutionText[i].setStyle(Text::Regular);
                    }
                    window.draw(resolutionText[i]);
                }
                resolutionText[chosen].setPosition(
                        screenW / 4, screenH / 6 + 6 * screenH / 10);
                resolutionText[chosen].setFillColor(Color::Green);
                window.draw(resolutionText[chosen]);
            } else {
                for (int i = 0; i < 4; i++) {
                    menuText[i].setPosition(
                            screenW / 4, screenH / 6 + i * screenH / 10);
                    if (i == activeMenu) {
                        menuText[i].setFillColor(Color::Red);
                        menuText[i].setStyle(Text::Bold);
                    } else {
                        menuText[i].setFillColor(Color::White);
                        menuText[i].setStyle(Text::Regular);
                    }
                    window.draw(menuText[i]);
                }
            }

            window.display();
        }
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
                Event event;
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
            Player** player = new Player*[2];
            player[0] = new Player1(
                    tiles,
                    screenW / 4 / TILE_SIZE * TILE_SIZE,
                    screenH / 2 / TILE_SIZE * TILE_SIZE);
            player[1] = new Player2(
                    tiles,
                    screenW / 4 * 3 / TILE_SIZE * TILE_SIZE,
                    screenH / 2 / TILE_SIZE * TILE_SIZE);
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
                    break;

                setParametrs(map, player, screenW, screenH);

                window.setView(view[0]);
                map.draw();
                player[1]->draw();
                player[0]->draw();
                draw(screenW, screenH, player, 0);
                //................

                window.setView(view[1]);
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
    Text scoreText("", font, screenW / 60 + TILE_SIZE / 4),
            bonusText("", font, screenW / 60 + TILE_SIZE / 4);
    scoreText.setFillColor(Color(77, 64, 37));
    bonusText.setFillColor(Color(77, 64, 37));

    ostringstream scoreStream, bonusStream;
    scoreStream << setfill('0') << setw(2) << player[currentPlayer]->getScore();

    scoreText.setString(L"Яблок собрано: " + scoreStream.str());

    scoreText.setPosition(
            view[currentPlayer].getCenter().x - screenW * 15 / 64 + TILE_SIZE,
            view[currentPlayer].getCenter().y - screenH / 2 + TILE_SIZE);
    window.draw(scoreText);
    if (!player[1 - currentPlayer]->getLife()
        && !player[1 - currentPlayer]->getWinner()) {
        player[1 - currentPlayer]->getSprite().setColor(Color::Black);
        player[currentPlayer]->getWinner() = true;
        player[currentPlayer]->getLife() = false;
    }
    if (player[currentPlayer]->getWinner()) {
        gameWon.setPosition(
                view[currentPlayer].getCenter().x - screenW * 7 / 32,
                view[currentPlayer].getCenter().y - screenH / 4);
        window.draw(gameWon);
    } else if (
            player[currentPlayer]->getLife()
            && player[currentPlayer]->getBonusTime() > 0) {
        bonusStream << setfill('0') << setw(2)
                    << player[currentPlayer]->getBonusLeft();
        bonusText.setString(L"Действие бонуса: " + bonusStream.str());
        bonusText.setPosition(
                view[currentPlayer].getCenter().x - screenW * 15 / 64
                        + TILE_SIZE,
                view[currentPlayer].getCenter().y - screenH / 2
                        + 2 * TILE_SIZE);
        window.draw(bonusText);
    } else if (!player[currentPlayer]->getLife()) {
        gameOver.setPosition(
                view[currentPlayer].getCenter().x - screenW / 9 * 2,
                view[currentPlayer].getCenter().y - screenH / 4);
        window.draw(gameOver);
    }
}
#endif
