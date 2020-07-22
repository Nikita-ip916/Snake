#include "SFML/Graphics.hpp"
#include "player.hpp"
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

using namespace std;
using namespace sf;

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    RenderWindow window(VideoMode(1920, 1080), "Snake 2020", Style::Fullscreen);
    view.reset(FloatRect(0, 0, 1920, 1080));

    Image mapImage;
    mapImage.loadFromFile("images/Tiles.png");
    Texture mapTexture;
    mapTexture.loadFromImage(mapImage);
    Sprite map;
    map.setTexture(mapTexture);

    Image heroImage;
    heroImage.loadFromFile("images/Tiles.png");

    Player p1(heroImage, "Player1", 320, 320, 32, 32);

    Clock clock;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        return 0;
    }
