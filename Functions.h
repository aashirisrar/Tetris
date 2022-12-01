#pragma once
#include "SFML/Graphics.hpp"
#include "Functions.h"
#include <iostream>
#include <time.h>
using namespace sf;


void Run(Clock &clock, RenderWindow &window, float &timer, bool &rotateCubes, int &movementOnXAxis, float &delayTimeToDescend)
{
    float time = clock.getElapsedTime().asSeconds();
    clock.restart();
    timer += time;

    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            window.close();
        }

        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Up)
            {
                rotateCubes = true;

            }
            else if (event.key.code == Keyboard::Left)
            {
                movementOnXAxis = -1;
            }
            else if (event.key.code == Keyboard::Right)
            {
                movementOnXAxis = 1;
            }
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Down))
    {
        delayTimeToDescend = 0.05;
    }
}