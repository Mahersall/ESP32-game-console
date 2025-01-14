#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class SnakeGame {
private:
    Adafruit_SSD1306* display;
    const int buttonTri, buttonX, buttonCircle, buttonSquare,buz;

    int snakeX[64];
    int snakeY[64];
    int snakeLength;
    int foodX, foodY;
    int directionX, directionY;
    bool gameOver;

    void updateSnake();
    bool checkCollision();
    void spawnFood();
    void drawSnake();
    void drawFood();
    void displayGameOver();
    void resetGame();

public:
    SnakeGame(Adafruit_SSD1306* disp, int tri, int x, int circle, int square,int buz);
    void playGame();
};

#endif
