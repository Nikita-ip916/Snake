#include "SFML/Graphics.hpp"
#include "map.hpp"
#include "player.hpp"
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace sf;

int main()
{
    // setlocale(LC_ALL, "Russian");
    // SetConsoleOutputCP(1251);
    // SetConsoleCP(1251);
    RenderWindow window(VideoMode(640, 480), "Snake 2020");
    // view.reset(FloatRect(0, 0, 640, 480));

    Image mapImage;
    mapImage.loadFromFile("images/tiles.png");
    Texture mapTexture;
    mapTexture.loadFromImage(mapImage);
    Sprite map;
    map.setTexture(mapTexture);

    Image heroImage;
    heroImage.loadFromFile("images/tiles.png");

    Player p1(heroImage, "Player1", 256, 128, 64, 64);

    Clock clock;
    while (window.isOpen()) {
        float time = clock.getElapsedTime().asMilliseconds();
        clock.restart();
        time = time / 100;
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        p1.update(time);
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
                p1.sprite.setTextureRect(IntRect(64, 0, 64, 64));
            } else if (p1.body[i].name == "Tail") {
                p1.sprite.setTextureRect(IntRect(256, 0, 64, 64));
            }
            p1.sprite.setPosition(p1.body[i].x, p1.body[i].y);
            window.draw(p1.sprite);
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }
        window.display();
    }
    return 0;
}
