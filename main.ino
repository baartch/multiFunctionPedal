#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// eigene Programm Dateien
#include "tuner.h"




#define MODE 1 // 0 = COMPRESSOR / 1 = TUNER

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region TUNER
// TUNER
float edgeL = 0; //set the lower edge of the spectrum
float edgeH = 0; //set the higher edge of the spectrum
float f;
#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region COMPRESSOR
// COMPRESSOR
#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region DISPLAY
// DISPLAY 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
extern Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
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
      initTuner();
      f = timer; //Datentyp 'float', wegen untenstehender Division
      f = (messzeit*4)/f; //Aus Periodendauer Frequenz berechnen
      detachInterrupt(digitalPinToInterrupt(2));
      ///////////////////////////////////////////////
      float avrgFreq = getAvrgFreq(f);
      display.setCursor(1,1);
      display.print(avrgFreq);
      

      for (int i=1; i < (getFreqArrSize())-1; i++){
        // Mitte zwischen aktuellem Ton und unterem und oberem berechnen, und alls Abgrenzung setzen
        edgeL = getEdgeL(i);
        edgeH = getEdgeH(i);
        if ((avrgFreq > edgeL) and (avrgFreq < edgeH)) {
          Serial.println(freeMemory());
          drawTunerPin(avrgFreq,getRefFreq(i));
          
          display.setCursor((SCREEN_WIDTH-36),1);
          display.print(getRefFreq(i));

          display.setCursor(59, 1);
          display.setTextSize(2);
          display.print(getNote(i));
          //display.drawChar
        }
      }
      display.display();
      ///////////////////////////////////////////////
      attachInterrupt(digitalPinToInterrupt(2), Messung, RISING); //Digital Pin 2
      zaehler = 0; //Frequenzzähler zurücksetzen
      startzeit = micros(); //Zeitpunkt der letzten Ausgabe speichern
    }
  }



}



void drawTunerPin (float frq, float ref){
  //calculate the total freq range on the display
  //take the higher distance to the next note
  float range = ((edgeH-ref)+(ref-edgeL))+abs((edgeH-ref)-(ref-edgeL));
  float frqInRng = frq - (ref-(range/2));
  int coordX = frqInRng * SCREEN_WIDTH / range;
  display.drawFastVLine(coordX, 20, 30, SSD1306_WHITE);
}

void initTuner(){
  display.clearDisplay();
  display.setTextSize(1);
  display.drawFastHLine(0,17,SCREEN_WIDTH,SSD1306_WHITE); //obere horizontale linie
  display.fillTriangle(0, SCREEN_HEIGHT-1, (SCREEN_WIDTH/2)-1, SCREEN_HEIGHT-10, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, SSD1306_WHITE);

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



#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}