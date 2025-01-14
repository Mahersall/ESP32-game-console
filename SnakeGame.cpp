#include "SnakeGame.h"



SnakeGame::SnakeGame(Adafruit_SSD1306* disp, int tri, int x, int circle, int square,int buz)
    : display(disp), buttonTri(tri), buttonX(x), buttonCircle(circle), buttonSquare(square),buz(buz)
      ,snakeLength(3), directionX(1), directionY(0), gameOver(false) {
    pinMode(buttonTri, INPUT_PULLUP);
    pinMode(buttonX, INPUT_PULLUP);
    pinMode(buttonCircle, INPUT_PULLUP);
    pinMode(buttonSquare, INPUT_PULLUP);
    pinMode(buz, OUTPUT);

    snakeX[0] = 64;
    snakeY[0] = 32;
    spawnFood();
}
void activateBuzzer(int buz) 
    {
    digitalWrite(buz, HIGH);
    delay(20);    
    digitalWrite(buz, LOW); 
    }

void SnakeGame::playGame() {
    if (gameOver) {
        displayGameOver();
        delay(2000);
        resetGame();
        return;
    }

    if (digitalRead(buttonTri) == HIGH && directionY == 0) {
        directionX = 0;
        directionY = -1;
    }
    if (digitalRead(buttonX) == HIGH && directionY == 0) {
        directionX = 0;
        directionY = 1;
    }
    if (digitalRead(buttonSquare) == HIGH && directionX == 0) {
        directionX = -1;
        directionY = 0;
    }
    if (digitalRead(buttonCircle) == HIGH && directionX == 0) {
        directionX = 1;
        directionY = 0;
    }

    updateSnake();

    if (checkCollision()) {
        gameOver = true;
        return;
    }

    if (snakeX[0] == foodX && snakeY[0] == foodY) {
        snakeLength++;
        activateBuzzer(buz);
        spawnFood();
    }

    display->clearDisplay();
    drawSnake();
    drawFood();
    display->display();

    delay(150);
}

void SnakeGame::updateSnake() {
    for (int i = snakeLength - 1; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }
    snakeX[0] += directionX * 4;
    snakeY[0] += directionY * 4;
}

bool SnakeGame::checkCollision() {
    if (snakeX[0] < 0 || snakeX[0] >= 128 || snakeY[0] < 0 || snakeY[0] >= 64)
        return true;

    for (int i = 1; i < snakeLength; i++) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
            return true;
    }
    return false;
}

void SnakeGame::spawnFood() {
    foodX = random(0, 128 / 4) * 4;
    foodY = random(0, 64 / 4) * 4;
}

void SnakeGame::drawSnake() {
    for (int i = 0; i < snakeLength; i++) {
        display->fillRect(snakeX[i], snakeY[i], 4, 4, WHITE);
    }
}

void SnakeGame::drawFood() {
    display->fillRect(foodX, foodY, 4, 4, WHITE);
}

void SnakeGame::displayGameOver() {
    display->clearDisplay();
    display->setTextSize(2);
    display->setTextColor(WHITE);
    display->setCursor(20, 20);
    display->println("Game Over");
    display->display();
}

void SnakeGame::resetGame() {
    snakeLength = 3;
    snakeX[0] = 64;
    snakeY[0] = 32;
    directionX = 1;
    directionY = 0;
    gameOver = false;
    spawnFood();
}
