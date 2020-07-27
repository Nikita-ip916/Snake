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
    mapImage.loadFromFile("images/Tiles.png");
    Texture mapTexture;
    mapTexture.loadFromImage(mapImage);
    Sprite map;
    map.setTexture(mapTexture);

    Image heroImage;
    heroImage.loadFromFile("images/Tiles.png");

    Player p1(heroImage, "Player1", 32, 32, 32, 32);

    Clock clock;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        for (int i = 0; i < HEIGHT_MAP; i++) {
            for (int j = 0; j < WIDTH_MAP; j++) {
                if (TileMap[i][j] == ' ') {
                    map.setTextureRect(IntRect(160, 0, 32, 32));
                }
                if (TileMap[i][j] == 'a') {
                    map.setTextureRect(IntRect(224, 0, 32, 32));
                }
                if (TileMap[i][j] == '0') {
                    map.setTextureRect(IntRect(192, 0, 32, 32));
                }
                if (TileMap[i][j] == '+') {
                    map.setTextureRect(IntRect(288, 0, 32, 32));
                }
                if (TileMap[i][j] == '-') {
                    map.setTextureRect(IntRect(256, 0, 32, 32));
                }
                map.setPosition(j * 32, i * 32);
                window.draw(map);
            }
        }
        for (unsigned int i = 0; i < p1.body.size(); i++) {
            if (p1.body[i].name == "Head") {
                p1.sprite.setTextureRect(IntRect(0, 0, 32, 32));
            } else if (p1.body[i].name == "Body") {
                p1.sprite.setTextureRect(IntRect(32, 0, 32, 32));
            } else if (p1.body[i].name == "Tail") {
                p1.sprite.setTextureRect(IntRect(128, 0, 32, 32));
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
