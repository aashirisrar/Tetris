#include "SFML/Graphics.hpp"
#include "Functions.h"
#include <iostream>
#include <time.h>
using namespace sf;

const int rowLength = 20;
const int coloumnLength = 10;

bool gameEnd;

int frameBlocks[rowLength][coloumnLength] = { 0 };

struct Cubes
{
    int x, y; // x and y components of the arrays a and b
} a[4], b[4]; // two global variables of struct Cubes named a and b of size 4

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

bool check();
void Run(Clock &clock, RenderWindow &window);
void Move();
void Rotate();
void Tick();
void CheckLines();
void Draw(RenderWindow& window, Sprite& s, Sprite& background, Sprite& frame, Text &gameOverText, Text &tetrisText);


int movementOnXAxis = 0;
bool rotateCubes = false;
int colorNumber = 1;
float timer = 0, delayTimeToDescend = 1;


int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(340, 544), "Tetris 2.0");
    //RenderWindow window(VideoMode(1280, 720), "Tetris 2.0");



    Texture textureTile, textureBackground, textureFrame;
    textureTile.loadFromFile("images/tilesnew.png");
    textureBackground.loadFromFile("images/b.png");
    textureFrame.loadFromFile("images/frame.png");

    Sprite tile(textureTile), background(textureBackground), frame(textureFrame);

    Font font;

    font.loadFromFile("fonts/gomarice_no_continue.ttf");

    Text gameOverText;
    Text tetrisText;

    // select the font
    gameOverText.setFont(font); 
    tetrisText.setFont(font);

    // set the string to display
    gameOverText.setString("GAME OVER");
    tetrisText.setString("TETRIS");

    // set the character size
    gameOverText.setCharacterSize(54);
    tetrisText.setCharacterSize(54);

    // set the color
    gameOverText.setFillColor(Color::Black);
    tetrisText.setFillColor(Color::Black);

    // set the text style
    gameOverText.setStyle(Text::Regular | Text::Underlined);
    tetrisText.setStyle(Text::Regular);

    // set the absolute position of the entity
    gameOverText.setPosition(52.5f, 465.f); // f for explicit typecasting
    tetrisText.setPosition(95.f, 15.f);

    // -------- Icon ---------
    Image icon;
    icon.loadFromFile("images/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    Clock clock;

    while (window.isOpen())
    {

        Run(clock, window);

        if (!gameEnd)
        {
            Move();

            Rotate();

            Tick();

            CheckLines();
        }
        
        Draw(window, tile, background, frame, gameOverText, tetrisText);
    }

    return 0;
}


bool check()
{
    for (int i = 0; i < 4; i++)
    {
        // not letting the tile escape the frame area at the side and at the bottom
        if (a[i].x < 0 || a[i].x >= coloumnLength || a[i].y >= rowLength)
        {
            return 0;
        }
        // not letting the tile to overlap or pass through another tile placed at a point
        else if (frameBlocks[a[i].y][a[i].x])
        {
            return 0;
        }
    }

    return 1;
}

//void Run(Clock &clock, RenderWindow &window)
//{
//    float time = clock.getElapsedTime().asSeconds();
//    clock.restart();
//    timer += time;
//
//    Event event;
//    while (window.pollEvent(event))
//    {
//        if (event.type == Event::Closed)
//        {
//            window.close();
//        }
//
//        if (event.type == Event::KeyPressed)
//        {
//            if (event.key.code == Keyboard::Up)
//            {
//                rotateCubes = true;
//
//            }
//            else if (event.key.code == Keyboard::Left)
//            {
//                movementOnXAxis = -1;
//            }
//            else if (event.key.code == Keyboard::Right)
//            {
//                movementOnXAxis = 1;
//            }
//        }
//    }
//
//    if (Keyboard::isKeyPressed(Keyboard::Down))
//    {
//        delayTimeToDescend = 0.05;
//    }
//}

void Move()
{
    //// <- Move -> ///
    for (int i = 0; i < 4; i++)
    {
        b[i] = a[i];
        a[i].x += movementOnXAxis;
    }
    if (!check())
    {
        for (int i = 0; i < 4; i++)
            a[i] = b[i];
    }
}

void Rotate()
{
    //////Rotate//////
    if (rotateCubes)
    {
        Cubes centerOfRotation = a[1]; // center of rotation
        for (int i = 0; i < 4; i++)
        {
            int x = a[i].y - centerOfRotation.y;
            int y = a[i].x - centerOfRotation.x;
            a[i].x = centerOfRotation.x - x;
            a[i].y = centerOfRotation.y + y;
        }
        if (!check())
        {
            for (int i = 0; i < 4; i++)
            {
                a[i] = b[i];
            }
        }
    }
}

void Tick()
{
    ///////Tick//////
    if (timer > delayTimeToDescend)
    {
        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i];
            a[i].y += 1;

        }

        if (!check())
        {
            for (int i = 0; i < 4; i++)
            {
                frameBlocks[b[i].y][b[i].x] = colorNumber;

            }

            colorNumber = 1 + rand() % 7;
            int n = rand() % 7;

            for (int i = 0; i < 4; i++)
            {
                a[i].x = tetriminoes[n][i] % 2;
                a[i].y = tetriminoes[n][i] / 2;
            }
        }

        timer = 0;
    }
}

void CheckLines()
{
    ///////check lines//////////
    int k = rowLength - 1;

    for (int i = rowLength - 1; i > 0; i--)
    {
        int count = 0;

        for (int j = 0; j < coloumnLength; j++)
        {
            if (frameBlocks[i][j])
            {
                count++;
            }

            frameBlocks[k][j] = frameBlocks[i][j];
        }
        if (count < coloumnLength)
        {
            k--;
        }
    }
            
    movementOnXAxis = 0;
    rotateCubes = 0;
    delayTimeToDescend = 0.3;
}

void Draw(RenderWindow &window, Sprite &tile, Sprite &background, Sprite &frame, Text &gameOverText, Text &tetrisText)
{
    /////////draw//////////
    window.clear(Color::White);
    window.draw(background);

    // displaying the blocks at bottom
    for (int i = 0; i < rowLength; i++)
    {
        for (int j = 0; j < coloumnLength; j++)
        {
            if (frameBlocks[i][j] == 0)
            {
                continue;
            }
            
            tile.setTextureRect(IntRect(frameBlocks[i][j] * 18, 0, 18, 18));
            tile.setPosition(j * 18, i * 18);
            tile.move(79.35, 95); // offset
            window.draw(tile);


            // Game Over Functionality
            if (tile.getPosition().y <= 120)
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
    window.display();
}