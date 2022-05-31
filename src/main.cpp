#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>

//Pre-processor variables
#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     4 
#define BUTTON_1 8
#define BUTTON_2 7
#define BUTTON_3 6

//Initialize the display:
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
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
  delay(3000);
  display.clearDisplay();
  display.setCursor(0, 20);
  display.setTextSize(1.8);
  display.write("Press the button\nunder the light that\nturns on first!\nYou have 100ms\nfor each turn.");
  display.display();
  delay(5000);
}

void loop() {


  display.drawCircle(display.width()/2, display.height()/2, 10, SSD1306_WHITE);
  display.drawCircle(display.width()/3, display.height()/2, 10, SSD1306_WHITE);
  display.drawCircle((display.width()/3)*3, display.height()/2, 10, SSD1306_WHITE);
}