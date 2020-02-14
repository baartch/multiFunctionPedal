#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// eigene Programm Dateien
#include "tuner.h"



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
extern Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define MODE 1 // 0 = COMPRESSOR / 1 = TUNER


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region DISPLAY
// DISPLAY 

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
  display.setTextColor(SSD1306_WHITE);
}








void loop()
{
  //Clear previous image.

  if (MODE == 0){ //COMPRESSOR
    display.clearDisplay();
    byte poti_threshold = (analogRead(A1)/4);
    byte poti_ratio = (analogRead(A2)/4);

    if (poti_threshold > 0){
      drawCompSettings(poti_threshold,poti_ratio);
    }

    display.display();
    delay(100);
  }
  
  //display.setCursor(0, 10);

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // TUNER
  if (MODE == 1){
    if ((micros() - startzeit) >= messzeit)
    {
      display.clearDisplay();
      float f = timer; //Datentyp 'float', wegen untenstehender Division
      f = 1000000/f; //Aus Periodendauer Frequenz berechnen
      detachInterrupt(0);
      display.setCursor(0, 10);
      display.print(f);
      display.display();

      attachInterrupt(digitalPinToInterrupt(2), Messung, RISING);
      zaehler = 0; //Frequenzzähler zurücksetzen
      startzeit = micros(); //Zeitpunkt der letzten Ausgabe speichern
    }
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
  display.fillTriangle(SCREEN_HEIGHT, SCREEN_HEIGHT, threshX, threshY, threshX, SCREEN_HEIGHT, SSD1306_WHITE);
  display.fillRect(threshX, threshY, (128-threshX), (SCREEN_HEIGHT-threshY), SSD1306_WHITE);

  //calculate ratio
  ratio = (((float)ratio*19)/potiMax)+1; // Die 20 ist das Maximum aus dem Compressor Script // 19+1 damit der Minimal-Wert 1 ist
  display.setCursor(SCREEN_WIDTH/2, 20);
  display.print(ratio);
  display.fillTriangle(threshX+1 , threshY-1, SCREEN_WIDTH, (threshY-((float)threshY/ratio)), SCREEN_WIDTH, threshY-1, SSD1306_WHITE);
}