#ifndef PONGGAME_H
#define PONGGAME_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class PongGame 
{
private:
    // Display reference
    Adafruit_SSD1306 *display;

    // Button pins
    int buttonSquare;
    int buttonCircle;
    int buttonHome;
    int joystickX;
    int buz;

    // Paddle settings
    const int paddleWidth = 20;
    const int paddleHeight = 5;
    int topPaddleX = 54;
    int bottomPaddleX = 54;
    const int paddleSpeed = 3;

    // Ball settings
    int ballX = 64;
    int ballY = 32;
    const int ballSize = 4;
    int ballSpeedX = 1;
    int ballSpeedY = 1;

    // Display winner and reset game
    void displayWinner(const char *winner) 
    {
        display->clearDisplay();
        display->setTextSize(1);
        display->setTextColor(WHITE);
        display->setCursor(10, 25);
        display->println(winner);
        display->display();
        delay(2000); 
        resetGame();
    }
    void activateBuzzer() 
    {
    digitalWrite(buz, HIGH); 
    delay(20);                   
    digitalWrite(buz, LOW); 
    }

    void resetGame() {
        topPaddleX = 54;
        bottomPaddleX = 54;
        ballX = 64;
        ballY = 32;
        ballSpeedX = 1;
        ballSpeedY = 1;
    }

public:
    PongGame(Adafruit_SSD1306 *disp, int btnSquare, int btnCircle, int btnHome, int joyX, int buzzerPin)
        : display(disp), buttonSquare(btnSquare), buttonCircle(btnCircle),
          buttonHome(btnHome), joystickX(joyX) , buz(buzzerPin) {}

    void playGame() {
        // Clear and prepare the display
        display->clearDisplay();

        // Move top paddle
        if (digitalRead(buttonCircle) == LOW) 
        {
            topPaddleX -= paddleSpeed;
            if (topPaddleX < 0) topPaddleX = 0;
        }
        if (digitalRead(buttonSquare) == LOW) 
        {
            topPaddleX += paddleSpeed;
            if (topPaddleX > 128 - paddleWidth) topPaddleX = 128 - paddleWidth;
        }

        // Move bottom paddle with joystick
        int xValue = analogRead(joystickX);
        if (xValue < 1500) {
            bottomPaddleX += paddleSpeed;
        } else if (xValue > 3000) {
            bottomPaddleX -= paddleSpeed;
        }
        if (bottomPaddleX < 0) bottomPaddleX = 0;
        if (bottomPaddleX > 128 - paddleWidth) bottomPaddleX = 128 - paddleWidth;

        // Move the ball
        ballX += ballSpeedX;
        ballY += ballSpeedY;

        // Ball collision with walls
        if (ballX <= 0 || ballX >= 128 - ballSize) {
            ballSpeedX = -ballSpeedX;
            activateBuzzer();
        }

        // Ball collision with top paddle
        if (ballY <= paddleHeight && ballX >= topPaddleX && ballX <= topPaddleX + paddleWidth) {
            ballSpeedY = -ballSpeedY;
            activateBuzzer();
        }

        // Ball collision with bottom paddle
        if (ballY >= 64 - paddleHeight - ballSize && ballX >= bottomPaddleX &&
            ballX <= bottomPaddleX + paddleWidth) {
            ballSpeedY = -ballSpeedY;
            activateBuzzer();
        }

        // Check for scoring conditions
        if (ballY <= 0) {
            displayWinner("Bottom Wins!");
        }
        if (ballY >= 64 - ballSize) {
            displayWinner("Top Wins!");
        }

        // Draw paddles and ball
        display->fillRect(topPaddleX, 0, paddleWidth, paddleHeight, WHITE);
        display->fillRect(bottomPaddleX, 64 - paddleHeight, paddleWidth, paddleHeight, WHITE);
        display->fillRect(ballX, ballY, ballSize, ballSize, WHITE);

        // Render everything to the screen
        display->display();

        // Check for exit condition
        if (digitalRead(buttonHome) == HIGH) 
        {
  
            resetGame();
        }
    }
};

#endif