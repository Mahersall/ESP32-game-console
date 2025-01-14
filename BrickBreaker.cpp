#include "BrickBreaker.h"



BrickBreaker::BrickBreaker(Adafruit_SSD1306* disp) 
    : display(disp), paddleX(54), paddleHeight(5), paddleWidth(20), 
      paddleSpeed(4), ballSize(4), brickWidth(12), brickHeight(4), gameOver(false) {
}

void BrickBreaker::initGame() 
{
    ballX = 64 - ballSize / 2;
    ballY = 45;
    ballSpeedX = 1;
    ballSpeedY = 1;
    paddleX = 54;

    int brickIndex = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 8; j++) {
            if (i == 4) continue;
            bricks[brickIndex][0] = j * (brickWidth + 2);
            bricks[brickIndex][1] = i * (brickHeight + 2) + 10;
            brickIndex++;
        }
    }
}

void BrickBreaker::playGame(int joystickValue, int homeButtonState,int buz) 
{
    pinMode(buz,OUTPUT);
    if (gameOver) 
    {
        display->setTextSize(2);
        display->setTextColor(WHITE);
        display->setCursor(35, 25);
        display->println(" XD");
        display->display();
        delay(2000);
        initGame();
        gameOver = false;
        return;
    }

    // Paddle control with joystick
    if (joystickValue < 1500) {
        paddleX += paddleSpeed;
    } else if (joystickValue > 3000) {
        paddleX -= paddleSpeed;
    }
    if (paddleX < 0) paddleX = 0;
    if (paddleX > 128 - paddleWidth) paddleX = 128 - paddleWidth;

    // Ball movement
    ballX += ballSpeedX;
    ballY += ballSpeedY;
    if (ballX <= 0 || ballX >= 128 - ballSize) ballSpeedX = -ballSpeedX;
    if (ballY <= 0) ballSpeedY = -ballSpeedY;

    // Ball and paddle collision
    if (ballY >= 64 - paddleHeight - ballSize && ballX >= paddleX && ballX <= paddleX + paddleWidth)
     {
        ballSpeedY = -ballSpeedY;
        digitalWrite(buz, HIGH);
        delay(20);              
        digitalWrite(buz, LOW);
    }

    // Ball and brick collision
    for (int i = 0; i < numBricks; i++) {
        if (bricks[i][0] != -1) {
            if (ballX >= bricks[i][0] && ballX <= bricks[i][0] + brickWidth &&
                ballY >= bricks[i][1] && ballY <= bricks[i][1] + brickHeight) {
                bricks[i][0] = -1;
                ballSpeedY = -ballSpeedY;
                digitalWrite(buz, HIGH);
                delay(20);              
                digitalWrite(buz, LOW);
            }
        }
    }

    // Game over condition
    if (ballY >= 64) {
        gameOver = true;
    }

    // Draw paddle, ball, and bricks
    display->clearDisplay();
    display->fillRect(paddleX, 60, paddleWidth, paddleHeight, WHITE);
    display->fillRect(ballX, ballY, ballSize, ballSize, WHITE);
    for (int i = 0; i < numBricks; i++) {
        if (bricks[i][0] != -1) {
            display->fillRect(bricks[i][0], bricks[i][1], brickWidth, brickHeight, WHITE);
        }
    }
    display->display();

    // Exit game logic
    if (homeButtonState == HIGH) 
    {
        gameOver = true; 
    }
    
}


bool BrickBreaker::isGameOver() const {
    return gameOver;
}
