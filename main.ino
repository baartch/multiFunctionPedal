// Adafruit GFX Library - Version: Latest
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>


// Adafruit SSD1306 - Version: 2.1.0
#include <Adafruit_SSD1306.h>
#include <splash.h>

// eigene Programm Dateien
#include "tuner.h"

#define MODE 1 // 0 = COMPRESSOR / 1 = TUNER


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region DISPLAY
// DISPLAY 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region TUNER variables
// TUNER

#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
void setup()
{
  Serial.begin(9600);
  //Initialize display by providing the display type and its I2C address.
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  //Set the text size and color.
  display.setTextSize(1);
  display.setTextColor(WHITE);
}








void loop()
{
  //Clear previous image.
  display.clearDisplay();

  if (MODE == 0){ //COMPRESSOR
    byte poti_threshold = (analogRead(A1)/4);
    byte poti_ratio = (analogRead(A2)/4);

    if (poti_threshold > 0){
      drawCompSettings(poti_threshold,poti_ratio);
    }

    delay(100);
  }
  
  display.setCursor(0, 10);

  if (MODE == 1){ //TUNER
    tuner();
  }




}


void drawCompSettings (byte thresh, float ratio) {
  // Variable declaration
  #define potiMax 255
  byte threshX;
  byte threshY;
  byte ratioX;
  byte ratioY;

  //calculate threshold
  byte thresh_percent = ((long)thresh*100)/potiMax;
  threshX = SCREEN_HEIGHT + ((thresh_percent * SCREEN_HEIGHT)/100);
  threshY = SCREEN_HEIGHT - ((thresh_percent * SCREEN_HEIGHT)/100);
  
  // draw Threshhold
  display.setCursor(SCREEN_WIDTH/2, 10);
  display.print(thresh);
  display.fillTriangle(SCREEN_HEIGHT, SCREEN_HEIGHT, threshX, threshY, threshX, SCREEN_HEIGHT, WHITE);
  display.fillRect(threshX, threshY, (128-threshX), (SCREEN_HEIGHT-threshY), WHITE);

  //calculate ratio
  ratio = (((float)ratio*19)/potiMax)+1; // Die 20 ist das Maximum aus dem Compressor Script // 19+1 damit der Minimal-Wert 1 ist
  display.setCursor(SCREEN_WIDTH/2, 20);
  display.print(ratio);
  display.fillTriangle(threshX+1 , threshY-1, SCREEN_WIDTH, (threshY-((float)threshY/ratio)), SCREEN_WIDTH, threshY-1, WHITE);
}