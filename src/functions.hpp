#include "SFML/Graphics.hpp"
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

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
///////////////////////////////////////////////////////////////////// Resourses
bool checkResourse(const char* file)
{
    FILE* resourse;
    resourse = fopen(file, "r");
    if (resourse) {
        fclose(resourse);
        return true;
    } else {
        cout << "Error: " << file << " not found. Exit programm.\n";
        return false;
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
    if (player[0]->getLife() && !player[0]->getWinner())
        if (player[1]->update(map, player[0]->getBody(), screenW, screenH)) {
            map.randomMapGenerate(
                    '-', player[1]->getBody(), player[0]->getBody());
            player[1]->getClockMove().restart();
        }
    player[0]->setBonusTimer();
    player[1]->setBonusTimer();
    window.clear(Color(77, 64, 37));
}
///////////////////////////////////////////////////////////////////////// Menu
void drawMenu(
        Map& map,
        int& screenW,
        int& screenH,
        bool& fullscreen,
        int& currentResolution,
        int* wins,
        string& plNumber)
{
    Text* menuText = new Text[4];
    Text* resolutionText = new Text[6];
    Text* roundsText = new Text[5];

    menuText[0].setString(L"* ИГРАТЬ ОДНОМУ");
    menuText[1].setString(L"* ИГРАТЬ С ДРУГОМ");
    menuText[2].setString(L"* РАЗРЕШЕНИЕ ЭКРАНА");
    menuText[3].setString(L"* ВЫХОД");

    resolutionText[0].setString(L"(800 x 600) SVGA");
    resolutionText[1].setString(L"(1280 x 720) HD");
    resolutionText[2].setString(L"(1600 x 900) WXGA++");
    resolutionText[3].setString(L"(1920 x 1080) Full HD");
    resolutionText[4].setString(L"Fullscreen    ---");
    resolutionText[5].setString(L"Применить изменения");

    roundsText[0].setString(L"Играть до:");
    roundsText[1].setString(L"* 1 победы");
    roundsText[2].setString(L"* 3 побед");
    roundsText[3].setString(L"* 5 побед");
    roundsText[4].setString(L"* 7 побед");

    Clock clockMenu;

    bool roundsMenu = false;
    int activeRounds = 1;

    bool resolutionMenu = false;
    bool tempFullscreen = fullscreen;
    int tempScreenW = screenW, tempScreenH = screenH;
    int activeResolution, chosen;
    activeResolution = chosen = currentResolution;

    int activeMenu = 0;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();

        float currentMenuDelay = clockMenu.getElapsedTime().asMilliseconds();

        for (int i = 0; i < 6; i++) {
            if (i < 4) {
                menuText[i].setFont(font);
                menuText[i].setCharacterSize(screenW / 30 + TILE_SIZE / 4);
            }
            if (i < 5) {
                roundsText[i].setFont(font);
                roundsText[i].setCharacterSize(screenW / 30 + TILE_SIZE / 4);
            }
            resolutionText[i].setFont(font);
            resolutionText[i].setCharacterSize(screenW / 30 + TILE_SIZE / 4);
        }

        map.clearMap();
        window.setView(window.getDefaultView());
        if (roundsMenu) {
            if (Keyboard::isKeyPressed(Keyboard::Escape)
                && currentMenuDelay > MOVE_DELAY / 3) {
                roundsMenu = false;
            } else if (
                    (Keyboard::isKeyPressed(Keyboard::Up)
                     || Keyboard::isKeyPressed(Keyboard::W))
                    && currentMenuDelay > MOVE_DELAY / 3) {
                if (activeRounds > 1) {
                    activeRounds--;
                } else
                    activeRounds = 4;
                clockMenu.restart();
            } else if (
                    (Keyboard::isKeyPressed(Keyboard::Down)
                     || Keyboard::isKeyPressed(Keyboard::S))
                    && currentMenuDelay > MOVE_DELAY / 3) {
                if (activeRounds < 4) {
                    activeRounds++;
                } else
                    activeRounds = 1;
                clockMenu.restart();
            } else if (
                    Keyboard::isKeyPressed(Keyboard::Return)
                    && currentMenuDelay > MOVE_DELAY / 3) {
                if (activeRounds == 1) {
                    wins[2] = 1;
                } else if (activeRounds == 2) {
                    wins[2] = 3;
                } else if (activeRounds == 3) {
                    wins[2] = 5;
                } else if (activeRounds == 4) {
                    wins[2] = 7;
                }
                roundsMenu = false;
                plNumber = "2";
                break;
                clockMenu.restart();
            }
        } else if (resolutionMenu) {
            if (Keyboard::isKeyPressed(Keyboard::Escape)
                && currentMenuDelay > MOVE_DELAY / 3) {
                activeResolution = chosen = currentResolution;
                resolutionMenu = false;
            } else if (
                    (Keyboard::isKeyPressed(Keyboard::Up)
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
                        currentResolution = activeResolution = chosen;
                        window.close();
                        if (fullscreen) {
                            window.create(
                                    VideoMode(screenW, screenH),
                                    "Snake 2020",
                                    Style::Fullscreen);
                        } else
                            window.create(
                                    VideoMode(screenW, screenH), "Snake 2020");
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
                    roundsMenu = true;
                } else if (activeMenu == 2) {
                    resolutionMenu = true;
                } else if (activeMenu == 3) {
                    window.close();
                }
                clockMenu.restart();
            }
        }
        map.draw();
        if (roundsMenu) {
            for (int i = 0; i < 5; i++) {
                roundsText[i].setPosition(
                        screenW / 7 * 2, screenH / 6 + i * screenH / 10);
                if (i == activeRounds) {
                    roundsText[i].setFillColor(Color::Red);
                    roundsText[i].setStyle(Text::Bold);
                } else {
                    roundsText[i].setFillColor(Color::White);
                    roundsText[i].setStyle(Text::Regular);
                }
                window.draw(roundsText[i]);
            }
        } else if (resolutionMenu) {
            if (tempFullscreen) {
                resolutionText[4].setString(L"Fullscreen    +++");
            } else {
                resolutionText[4].setString(L"Fullscreen    ---");
            }
            for (int i = 0; i < 6; i++) {
                resolutionText[i].setPosition(
                        screenW / 7 * 2, screenH / 6 + i * screenH / 10);
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
                    screenW / 7 * 2, screenH / 6 + 6 * screenH / 10);
            resolutionText[chosen].setFillColor(Color::Green);
            window.draw(resolutionText[chosen]);
        } else {
            for (int i = 0; i < 4; i++) {
                menuText[i].setPosition(
                        screenW / 7 * 2, screenH / 6 + i * screenH / 10);
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
}

#endif
