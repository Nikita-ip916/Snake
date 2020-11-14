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

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

void setParametrs(Map&, Player&, int, int);
void setParametrs(Map&, Player**, int, int);
void draw(int, int, Player**, int*, int);

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

    bool resolutionMenu = false, fullscreen = false;
    bool tempFullscreen = fullscreen;
    int activeResolution, currentResolution = 3, chosen;
    activeResolution = chosen = currentResolution;
    int tempScreenW = screenW, tempScreenH = screenH;

    int activeMenu = 0;

    while (window.isOpen()) {
        // drawMenu(bool &fullscreen, int&currentResolution);

        while (window.isOpen()) {
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
                if (i < 5) {
                    roundsText[i].setFont(font);
                    roundsText[i].setCharacterSize(
                            screenW / 30 + TILE_SIZE / 4);
                }
                resolutionText[i].setFont(font);
                resolutionText[i].setCharacterSize(
                        screenW / 30 + TILE_SIZE / 4);
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
                    draw(screenW, screenH, player, wins, 0);

                    window.setView(view[1]);
                    map.draw();
                    player[0]->draw();
                    player[1]->draw();
                    draw(screenW, screenH, player, wins, 1);

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
void draw(
        int screenW, int screenH, Player** player, int* wins, int currentPlayer)
{
    Text winsText("", font, screenW / 50 + TILE_SIZE / 5),
            scoreText("", font, screenW / 50 + TILE_SIZE / 5),
            bonusText("", font, screenW / 50 + TILE_SIZE / 5);
    winsText.setFillColor(Color(77, 64, 37));
    scoreText.setFillColor(Color(77, 64, 37));
    bonusText.setFillColor(Color(77, 64, 37));

    ostringstream winsStream, totWinsStream, scoreStream, bonusStream;

    winsStream << setw(1) << wins[currentPlayer];
    totWinsStream << setw(1) << wins[2];
    scoreStream << setfill('0') << setw(2) << player[currentPlayer]->getScore();

    scoreText.setString(L"Яблок собрано: " + scoreStream.str());
    winsText.setString(
            L"Побед в раундах: " + winsStream.str() + L" из "
            + totWinsStream.str());

    winsText.setPosition(
            view[currentPlayer].getCenter().x - screenW * 15 / 64 + TILE_SIZE,
            view[currentPlayer].getCenter().y - screenH / 2 + TILE_SIZE);
    window.draw(winsText);

    scoreText.setPosition(
            view[currentPlayer].getCenter().x - screenW * 15 / 64 + TILE_SIZE,
            view[currentPlayer].getCenter().y - screenH / 2 + 2 * TILE_SIZE);
    window.draw(scoreText);

    if (!player[1 - currentPlayer]->getLife()
        && !player[1 - currentPlayer]->getWinner()) {
        player[1 - currentPlayer]->getSprite().setColor(Color::Black);
        player[currentPlayer]->getWinner() = true;
        player[currentPlayer]->getLife() = false;
    }
    if (player[currentPlayer]->getWinner()) {
        player[1 - currentPlayer]->getLife() = false;
        gameWon.setPosition(
                view[currentPlayer].getCenter().x - screenW / 7,
                view[currentPlayer].getCenter().y - screenH / 9 * 2);
        gameWon.setCharacterSize(screenW / 20 + TILE_SIZE / 4);
        if (wins[currentPlayer] >= wins[2]) {
            gameWon.setString(L"      Вы\nпобедили");
        } else {
            gameWon.setString(L"  Раунд\nвыигран\n (Enter)");
        }
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
                        + 3 * TILE_SIZE);
        window.draw(bonusText);
    } else if (!player[currentPlayer]->getLife()) {
        gameOver.setPosition(
                view[currentPlayer].getCenter().x - screenW / 7,
                view[currentPlayer].getCenter().y - screenH / 9 * 2);
        gameOver.setCharacterSize(screenW / 20 + TILE_SIZE / 4);
        if (wins[1 - currentPlayer] >= wins[2]) {
            gameOver.setString(L"    Игра\nокончена");
        } else {
            gameOver.setString(L"  Раунд\nпроигран\n (Enter)");
        }
        window.draw(gameOver);
    }
}
#endif
