#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>

//Pre-processor variables
#define SCREEN_ADDRESS 0x3C;
#define SCREEN_WIDTH 128;
#define SCREEN_HEIGHT 64;
#define OLED_RESET     4 ;
#define BUTTON_1 8;
#define BUTTON_2 7;
#define BUTTON_3 6;

//Initialize the display:
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
}

void loop() {

}