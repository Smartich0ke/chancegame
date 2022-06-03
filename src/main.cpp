//include the libraries
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>

//Pre-processor variables:
#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     4 
#define BUTTON_1 8
#define BUTTON_2 7
#define BUTTON_3 6

//Timer variables:
int startTime;
int stopTime;
int currTime;

//Button states variables:
int button1State;
int button2State;
int button3State;

//Circle variables:
int pickedCircle;
int buttonPressed;

//Initialize the display:
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class scoreboard {
  public:
    //Rounds and scores variables:
    int arduinoPoints;
    int userPoints;
    int totalPoints;
    int rounds;
    void displayScore() {
      display.clearDisplay();
      display.setCursor(0, 4);
      display.setTextSize(1.8);
      display.print("Score:");
      display.setCursor(0,20);
      display.print("Computer     ");
      display.print("Human");
      display.setCursor(0, 35);
      display.setTextSize(3);
      display.print(" ");
      display.print(arduinoPoints);
      Serial.print(arduinoPoints);
      display.print("   ");
      display.print(userPoints);
      display.display();
    }
    scoreboard() {
    rounds = 0;
    userPoints = 0;
    arduinoPoints = 0;
    totalPoints = 0;
    rounds = 0;
  }
};

scoreboard currScore;

void setup() {
  //stop and loop forever if display allocation fails
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  //Start a serial connetion for debugging
  Serial.begin(9600);

  //Set the button pins as input pullups
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);

  //A0 is left floating and will act as our source of randomness
  pinMode(A0, INPUT);

  //Show a splash screen for 3 secs
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  //display.cp437(true);
  display.print("Chance\n  v1.0.0"); 
  display.setCursor(0, 40);
  display.setTextSize(1.2);
  display.print("    By Nikolai");
  display.display();
  delay(2000);

  //Display instructions on how to play:
  display.clearDisplay();
  display.setCursor(0, 20);
  display.setTextSize(1.8);
  display.print("Press the button\nunder the light that\nturns on first!\nYou have 750ms\nfor each turn.");
  display.display();
  startTime = millis();
  stopTime = startTime + 10000;
  for (currTime = 0; currTime <= stopTime;) {
    currTime = millis();
    button1State = digitalRead(BUTTON_1);
    if (button1State == 0) {
      currTime = stopTime;
    }
  }
  display.clearDisplay();
  display.setCursor(0, 20);
  display.print("Remember, to to exit the scoreboard press the far left button");
  display.display();
  stopTime = startTime + 10000;
  for (currTime = 0; currTime <= stopTime;) {
    currTime = millis();
    button1State = digitalRead(BUTTON_1);
    if (button1State == 0) {
      currTime = stopTime;
    }
  }
  

}

void loop() {
  delay(250);
  button1State = 1;
  button2State = 1;
  button3State = 1;
  //Draw unfilled circles and display "Get ready!!" text:
  display.clearDisplay();
  display.setCursor(0, 4);
  display.setTextSize(1.8);
  display.print("Get ready!!");
  display.drawCircle(display.width()/2, display.height()/2, 10, SSD1306_WHITE);
  display.drawCircle((display.width()/3)-15, display.height()/2, 10, SSD1306_WHITE);
  display.drawCircle(((display.width()/3)*2)+15, display.height()/2, 10, SSD1306_WHITE);
  display.display();

  //Read the floating analog pin for randomness
  randomSeed(analogRead(A0));

  //Run an interrupt timer for a semi-random amount of time:
  startTime = millis();
  stopTime = startTime + random(500, 5000);
  for (currTime = 0; currTime <= stopTime;) {
    currTime = millis();

    //Read the button pins and update the status variables:
    button1State = digitalRead(BUTTON_1);
    button2State = digitalRead(BUTTON_2);
    button3State = digitalRead(BUTTON_3);

    //if someone presses a button to early, the Arduino gains a point:
    if (button1State == 0 || button2State == 0 || button3State == 0) {
      display.clearDisplay();
      display.setCursor(4, 4);
      display.print("too early!!");
      display.display();
      delay(1000);
      currScore.arduinoPoints++;
      currScore.rounds++;
      currScore.displayScore();
      currTime = stopTime;
      button1State = 1;
      button2State = 1;
      button3State = 1;
      while (button1State == 1) {
        button1State = digitalRead(BUTTON_1);
      }
      return;
    }
  }
  display.clearDisplay();
  display.setCursor(4, 4);
  display.print("go!!");
  display.display();
  randomSeed(analogRead(A0));
  pickedCircle = random(1, 4); //Remember, the max is exclusive so really this is picking a number 1, 3 or 3
  switch (pickedCircle) {
  case 1:
    display.fillCircle((display.width()/3)-15, display.height()/2, 10, SSD1306_WHITE);
    display.drawCircle(display.width()/2, display.height()/2, 10, SSD1306_WHITE);
    display.drawCircle(((display.width()/3)*2)+15, display.height()/2, 10, SSD1306_WHITE);
    display.display();
  break;
  case 2:
    display.fillCircle(display.width()/2, display.height()/2, 10, SSD1306_WHITE);
    display.drawCircle(((display.width()/3)*2)+15, display.height()/2, 10, SSD1306_WHITE);
    display.drawCircle((display.width()/3)-15, display.height()/2, 10, SSD1306_WHITE);
    display.display();
  break;
  case 3:
    display.drawCircle((display.width()/3)-15, display.height()/2, 10, SSD1306_WHITE);
    display.drawCircle(display.width()/2, display.height()/2, 10, SSD1306_WHITE);
    display.fillCircle(((display.width()/3)*2)+15, display.height()/2, 10, SSD1306_WHITE);
    display.display();
  break;
  }
  buttonPressed = 0;
  startTime = millis();
  stopTime = startTime + 500;
  for (currTime = 0; currTime <= stopTime;){
    currTime = millis();
    button1State = digitalRead(BUTTON_1);
    button2State = digitalRead(BUTTON_2);
    button3State = digitalRead(BUTTON_3);
    if (button1State == 0) {
      buttonPressed = 1;
    }
    if (button2State == 0) {
      buttonPressed = 2;
    }
    if (button3State == 0) {
      buttonPressed = 3;
    }
    if (buttonPressed == pickedCircle) {
      display.clearDisplay();
      display.setTextSize(1.8);
      display.setCursor (4, 4);
      display.print("User wins!!");
      display.display();
      delay(1000);
      currScore.userPoints++;
      currScore.displayScore();
      button1State = 1;
      button2State = 1;
      button3State = 1;
      while (button1State == 1) {
        button1State = digitalRead(BUTTON_1);
      }
      return;
    }
    else if (buttonPressed > 0 && buttonPressed != pickedCircle) {
      display.clearDisplay();
      display.print("wrong button!!\n\nPress the middle button to continue");
      display.display();
      delay(2000);
      currScore.arduinoPoints++;
      currScore.displayScore();
      button1State = 1;
      button2State = 1;
      button3State = 1;
      while (button1State == 1) {
        button1State = digitalRead(BUTTON_1);
      }
      return;
    }
  }
  display.clearDisplay();
  display.setCursor(4, 4);
  display.setTextSize(1.8);
  display.print("too late!!");
  display.display();
  delay(250);
  currScore.arduinoPoints++;
  currScore.displayScore();
  while (button1State == 1) {
    button1State = digitalRead(BUTTON_1);
  }
  startTime = millis();
  stopTime = startTime + 2000;
  for (currTime = 0; currTime <= stopTime;) {
    currTime = millis();
    button1State = digitalRead(BUTTON_1);
    if (button1State == 0) {
      return;
    }
    
  }
}
