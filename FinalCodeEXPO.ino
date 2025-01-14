#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BrickBreaker.h"
#include "SnakeGame.h"
#include "PongGame.h" 

const int buzzerPin = 25;


Adafruit_SSD1306 myDisplay(128, 64, &Wire);


const int button_tri = 19;    
const int button_x = 17;      
const int button_circle = 18; 
const int button_square = 4;
const int button_home = 5;    
#define VRx1 34              

// Menu variables
const char *menuItems[] = {"Pong", "Tetris", "Snake", "Brick Breaker"};
const int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
int currentSelection = 0;
bool inGame = false;

// Game variables
BrickBreaker *brickBreakerGame = nullptr; 
SnakeGame *snakeGame = nullptr;          
PongGame *pongGame = nullptr;           

void setup() 
{
  pinMode(buzzerPin, OUTPUT);
  // Initialize OLED display
  myDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  myDisplay.clearDisplay();
  myDisplay.display();

  // Initialize button pins
  pinMode(button_tri, INPUT_PULLUP);
  pinMode(button_x, INPUT_PULLUP);
  pinMode(button_circle, INPUT_PULLUP);
  pinMode(button_home, INPUT_PULLUP);
  pinMode(button_square, INPUT_PULLUP);
  pinMode(VRx1, INPUT);

  // Initialize Serial for debugging
  Serial.begin(9600);
}

void loop() {
  myDisplay.clearDisplay();

  if (pongGame != nullptr) 
  {
    Serial.println("PongGame is active.");
    pongGame->playGame();
    if (digitalRead(button_home) == HIGH) 
    {
        Serial.println("Exiting PongGame...");
        delete pongGame; 
        pongGame = nullptr;
        delay(200); 
    }
    return;
}

  if (brickBreakerGame != nullptr) 
  {
    // If playing Brick Breaker, update the game logic
    int joystickValue = analogRead(VRx1);
    int homeButtonState = digitalRead(button_home);
    brickBreakerGame->playGame(joystickValue, homeButtonState,buzzerPin);
    if (homeButtonState == HIGH) 
    {
      delete brickBreakerGame; 
      brickBreakerGame = nullptr;
      delay(200);
    }
    return;
  }

  if (snakeGame != nullptr) {
    // If playing Snake, update the game logic
    snakeGame->playGame();
    if (digitalRead(button_home) == HIGH) {
      delete snakeGame; 
      snakeGame = nullptr;
      delay(200); 
    }
    return;
  }

  if (inGame) {
    // Display "Coming Soon" screen for the selected game
    myDisplay.setTextColor(WHITE);
    myDisplay.setTextSize(1);
    myDisplay.setCursor(10, 20);
    myDisplay.println(menuItems[currentSelection]);
    myDisplay.setCursor(10, 40);
    myDisplay.println("is coming soon!");

    // Check for the home button to return to the main menu
    if (digitalRead(button_home) == HIGH) {
      inGame = false;
      delay(200); 
    }
  } else {
    // Display the main menu
    for (int i = 0; i < menuSize; i++) {
      if (i == currentSelection) {
        // Highlight the selected menu item
        myDisplay.setTextColor(BLACK, WHITE); 
      } else {
        myDisplay.setTextColor(WHITE, BLACK);
      }
      myDisplay.setTextSize(1);
      myDisplay.setCursor(10, 10 + i * 12);
      myDisplay.println(menuItems[i]);
    }

    // Check for button presses to navigate the menu
    if (digitalRead(button_x) == LOW) {
      currentSelection--;
      if (currentSelection < 0) {
        currentSelection = menuSize - 1;
      }
      delay(200);
    }

    if (digitalRead(button_tri) == LOW) {
      currentSelection++;
      if (currentSelection >= menuSize) {
        currentSelection = 0; 
      }
      delay(200); 
    }

    // Check for the circle button to enter a game
    if (digitalRead(button_circle) == HIGH) 
    {
      if (currentSelection == 0) 
      { 
        Serial.println("Starting Pong game...");
        pongGame = new PongGame(&myDisplay, button_square, button_circle, button_home, VRx1,buzzerPin);
      } else if (currentSelection == 3) 
      {
        brickBreakerGame = new BrickBreaker(&myDisplay);
      } else if (currentSelection == 2) 
      {
        snakeGame = new SnakeGame(&myDisplay, button_tri, button_x, button_circle, button_square,buzzerPin);
      } else 
      {
        inGame = true;
      }
      delay(200);
    }
  }

  myDisplay.display();
}
