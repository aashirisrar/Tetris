#pragma once
#include "SFML/Graphics.hpp"
#include <time.h>
using namespace sf;

const int rowLength = 20; //---------------------------------------------------------------rows of frame
const int coloumnLength = 10;  //---------------------------------------------------------------columns of frame
int frameBlocks[rowLength][coloumnLength] = { 0 }; // ----------------------------------------explain why the const is writtern here

int tetriminoes[7][4] = // array of predefined tetriminoes shapes
{
    {3, 5, 4, 6}, // S
    {2, 3, 5, 7}, // L
    {1, 3, 5, 7}, // I
    {3, 5, 4, 7}, // T
    {2, 3, 4, 5}, // O
    {3, 5, 7, 6}, // J
    {2, 4, 5, 7}, // Z
};

struct Cubes
{
    int x, y; // x and y components of the arrays a and b
} a[4], b[4]; // two global variables of struct Cubes named a and b of size 4 ----------------------------------------- 2 array are used in case the cubes are touching with frame and the user still
                                                                                                                     // presses the left right or up key, we make a copy of the a struct which is our main 
                                                                                                                   // and revert it back in case there is no block available in the frame for the cubes.
/*
    all loops have i<4 because we have always 4 blocks to handle (tetris rule) ----- 0 -> 3 (array)
*/



bool CheckFrame()
{
    for (int i = 0; i < 4; i++)
    {
        // not letting the tile escape the frame area at the side and at the bottom
        if (a[i].x < 0 || a[i].x >= coloumnLength || a[i].y >= rowLength)
        {
            return 0; // false
        }
        // not letting the tile to overlap or pass through another tile placed at a point
        else if (frameBlocks[a[i].y][a[i].x])  // -------------------------------------------- to do
        {
            return 0;  // false
        }
    }
    return 1;    // true
} 

void RunGame(Clock& clock, RenderWindow& window, float& timer, bool& rotateCubes, int& movementOnXAxis, float& delayTimeToDescend)
{
    float time = clock.getElapsedTime().asSeconds(); // ---------------------------------------------- getting random time at every instance 
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

void Movement(int &movementOnXAxis )
{  
    for (int i = 0; i < 4; i++)
    {
        b[i] = a[i];  // b is a temp variable  ------------------------------- making a copy of the original array of blocks
        a[i].x += movementOnXAxis;  //---------------------------------------- adding or subtracting its movement on x axis according to value recieved from the run function
    }
    if (!CheckFrame()) // ------------------------------------------------reverting the blocks back as there is no space for the blocks at the sides to move
    {
        for (int i = 0; i < 4; i++)
            a[i] = b[i];
    }
}

void Rotation(bool &rotateCubes)
{
    if (rotateCubes)
    {
        Cubes centerOfRotation = a[1]; // struct Cubes -> center of rotation   
        for (int i = 0; i < 4; i++)
        {
            int x = a[i].y - centerOfRotation.y;
            int y = a[i].x - centerOfRotation.x;
            a[i].x = centerOfRotation.x - x;
            a[i].y = centerOfRotation.y + y;
        }
        if (!CheckFrame())
        {
            for (int i = 0; i < 4; i++)
            {
                a[i] = b[i];  // ------------------------------------------------reverting the blocks back as there is no space for the blocks at the sides to rotate
            }
        }
    }
}

void CheckRows(bool &rotateCubes, int &movementOnXAxis, float &delayTimeToDescend)
{
    int k = rowLength - 1;

    for (int i = rowLength - 1; i > 0; i--)
    {
        int count = 0;

        for (int j = 0; j < coloumnLength; j++)
        {
            if (frameBlocks[i][j])
            {
                count++;                                // ----------------- counting the number of columns filled in a row
            }

            frameBlocks[k][j] = frameBlocks[i][j];     // ------------------ if entire row is filled remove that array
        }
        if (count < coloumnLength)
        {
            k--;
        }
    }
            
    movementOnXAxis = 0;      //-----------------------------------------------------------------------------------------------------------------------------------------
    rotateCubes = 0;            //---------------------------------------------------- reverting the variables back to original so they dont loop over and over again
    delayTimeToDescend = 0.3;  //----------------------------------------------------------------------------------------------------------------------------------------
}

void DescendTile(float& timer, float& delayTimeToDescend, int& colorNumber)
{
    if (timer > delayTimeToDescend)    // --------------------------- checking so that it does not move the tile down infinitely
    {
        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i]; // -------------------------------------------------- making copy (temp variable)
            a[i].y += 1; //---------------------------------------------------- adding 1 on y to move the blocks downward
        }

        if (!CheckFrame())
        {
            for (int i = 0; i < 4; i++)
            {
                frameBlocks[b[i].y][b[i].x] = colorNumber;  //---------------------------------------------------- reverting the variables back to original so they dont loop over and over again
            }

            colorNumber = 1 + rand() % 7;  // ------------------------------------------ getting a random color 
            int n = rand() % 7;           // ------------------------------------------ handling the shapes

            for (int i = 0; i < 4; i++)
            {
                a[i].x = tetriminoes[n][i] % 2;      // ----------------------------------- building the shapes from the array on the x and y axis
                a[i].y = tetriminoes[n][i] / 2;
            }
        }

        timer = 0;   //---------------------------------------------------- reverting time variable back to zero so it does not add up again and again infinitely (this will make the blocks spawn very fast)
    }
}

void DrawGraphics(RenderWindow& window, Sprite& tile, Sprite& background, Sprite& frame, Text& gameOverText, Text& tetrisText, bool& gameEnd, int& colorNumber)
{
    window.clear(Color::White);    // ------------------------------- clearing window and giving a white color to it
    window.draw(background);  // drawing the background sprite

    // displaying the blocks at bottom
    for (int i = 0; i < rowLength; i++)
    {
        for (int j = 0; j < coloumnLength; j++)
        {
            if (frameBlocks[i][j] == 0) // -------------------------------------------------------------- checking if there is already a block present at that point 
            {
                continue;    
            }

            tile.setTextureRect(IntRect(frameBlocks[i][j] * 18, 0, 18, 18)); // ---------------------------- divide the tile texture to show only 1 rectangle
            tile.setPosition(j * 18, i * 18); //----------------------absolute position (according to pc)
            tile.move(79.35, 95); // offset  --------------- relative position (according to us) (visible)
            window.draw(tile);

            // Game Over Functionality
            if (tile.getPosition().y <= 120)  // -------------------------- taking the y coordinate of the application window from the top
            {
                window.draw(gameOverText);
                gameEnd = true;
            }
        }

    }

    // spawning new blocks
    for (int i = 0; i < 4; i++)
    {
        tile.setTextureRect(IntRect(colorNumber * 18, 0, 18, 18));
        tile.setPosition(a[i].x * 18, a[i].y * 18);
        tile.move(79.35, 95); // offset
        window.draw(tile);
    }

    frame.setPosition(50, 65);
    window.draw(frame);

    window.draw(tetrisText);
    window.display();            // ---------------------------- display all the graphics in the window
}
