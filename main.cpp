#include "SFML/Graphics.hpp"
#include "functions.h"
#include <time.h>
using namespace sf;

// Function Prototypes
bool check();
void Run(Clock &clock, RenderWindow &window, float &timer, bool &rotateCubes, int &movementOnXAxis, float &delayTimeToDescend);
void Move(int& movementOnXAxis);
void Rotate(bool& rotateCubes);
void Tick(float& timer, float& delayTimeToDescend, int& colorNumber);
void CheckLines(bool& rotateCubes, int& movementOnXAxis, float& delayTimeToDescend);
void Draw(RenderWindow &window, Sprite &tile, Sprite &background, Sprite &frame, Text &gameOverText, Text &tetrisText, bool &gameEnd, int &colorNumber);

// Global Variable Declarations
int movementOnXAxis = 0;
bool rotateCubes = false;
int colorNumber = 1;
float timer = 0;
float delayTimeToDescend = 1;
bool gameEnd;


int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(340, 544), "Tetris 2.0");

    /* --------Sprite-------- */
    Texture textureTile, textureBackground, textureFrame;
    textureTile.loadFromFile("images/tilesnew.png");
    textureBackground.loadFromFile("images/b.png");
    textureFrame.loadFromFile("images/frame.png");

    Sprite tile(textureTile), background(textureBackground), frame(textureFrame);

    /* --------Text-------- */
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
    gameOverText.setStyle(Text::Regular );
    tetrisText.setStyle(Text::Regular);

    // set the absolute position of the entity
    gameOverText.setPosition(52.5f, 465.f); // f for explicit typecasting
    tetrisText.setPosition(95.f, 15.f);

    /* --------Icon-------- */
    Image icon;
    icon.loadFromFile("images/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    Clock clock;

    while (window.isOpen())
    {
        Run(clock, window, timer, rotateCubes, movementOnXAxis, delayTimeToDescend);

        if (!gameEnd)
        {
            Move(movementOnXAxis);

            Rotate(rotateCubes);

            Tick(timer, delayTimeToDescend, colorNumber);

            CheckLines(rotateCubes,  movementOnXAxis, delayTimeToDescend);
        }
        
        Draw(window, tile, background, frame, gameOverText, tetrisText, gameEnd, colorNumber);

    }

    return 0;
}
