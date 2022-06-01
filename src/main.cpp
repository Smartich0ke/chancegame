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

//Rounds and scores variables:
int arduinoPoints;
int userPoints;
int totalPoints;
int rounds;
//Initialize the display:
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  //stop and loop forever if display allocation fails
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
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
  display.cp437(true);
  display.write(" Chance\n  v0.1.0"); 
  display.setCursor(0, 40);
  display.setTextSize(1.2);
  display.write("    By Nikolai");
  display.display();
  delay(2000);

  //Display instructions on how to play:
  display.clearDisplay();
  display.setCursor(0, 20);
  display.setTextSize(1.8);
  display.write("Press the button\nunder the light that\nturns on first!\nYou have 100ms\nfor each turn.");
  display.display();
  delay(1000);

}

void loop() {
  //Draw unfilled circles and display "Get ready!!" text:
  display.clearDisplay();
  display.setCursor(0, 4);
  display.setTextSize(1.8);
  display.write("Get ready!!");
  display.drawCircle(display.width()/2, display.height()/2, 10, SSD1306_WHITE);
  display.drawCircle((display.width()/3)-15, display.height()/2, 10, SSD1306_WHITE);
  display.drawCircle(((display.width()/3)*2)+15, display.height()/2, 10, SSD1306_WHITE);
  display.display();

  //Read the floating analog pin for randomness
  randomSeed(analogRead(A0));

  //Run an interrupt timer for a semi-random amount of time:
  startTime = millis();
  stopTime = startTime + random(500, 5000);
  for (currTime = 0; currTime <= stopTime;){
    currTime = millis();

    //Read the button pins and update the status variables:
    button1State = digitalRead(BUTTON_1);
    button2State = digitalRead(BUTTON_2);
    button3State = digitalRead(BUTTON_3);

    //if someone presses a button to early, the Arduino gains a point:
    if (button1State == 0 || button2State == 0 || button3State == 0) {
      display.clearDisplay();
      display.fillCircle((display.width()/3)-15, display.height()/2, 10, SSD1306_WHITE);
      display.display();
      delay(200);
      display.fillCircle(display.width()/2, display.height()/2, 10, SSD1306_WHITE);
      display.display();
      delay(200);
      display.fillCircle(((display.width()/3)*2)+15, display.height()/2, 10, SSD1306_WHITE);
      display.display();
      display.setCursor(0, 4);
      display.setTextSize(1.8);
      display.write("Ahhh. Too fast!!");
      display.display();
      delay(500);
      arduinoPoints++;
      display.clearDisplay();
      display.setCursor(0, 4);
      display.setTextSize(1.8);
      display.write("Score:");
      display.setCursor(0, 0);
      display.write("\n");
      display.write(arduinoPoints);
      display.write("   ");
      display.write(userPoints);
      display.display();
      delay(1000);
    }
  }
}
