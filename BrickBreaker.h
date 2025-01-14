#ifndef BRICKBREAKER_H
#define BRICKBREAKER_H

#include <Adafruit_SSD1306.h>

class BrickBreaker {
private:
    Adafruit_SSD1306* display; // Pointer to the OLED display
    int paddleX, paddleHeight, paddleWidth, paddleSpeed;
    int ballX, ballY, ballSize, ballSpeedX, ballSpeedY;
    int bricks[40][2]; 
    int brickWidth, brickHeight;
    bool gameOver;
    const int numBricks = 40;

public:
    BrickBreaker(Adafruit_SSD1306* disp);

    void initGame();
    void playGame(int joystickValue, int homeButtonState,int buz);
    bool isGameOver() const;
};

#endif
