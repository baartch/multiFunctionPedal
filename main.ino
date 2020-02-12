// Adafruit GFX Library - Version: Latest
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>


// Adafruit SSD1306 - Version: Latest
#include <Adafruit_SSD1306.h>
#include <splash.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region DISPLAY
// DISPLAY 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region TUNER
// TUNER 
//clipping indicator variables
boolean clipping = 0;

//data storage variables
byte newData = 0;
byte prevData = 0;
unsigned int time = 0;//keeps time and sends vales to store in timer[] occasionally
int timer[10];//storage for timing of events
int slope[10];//storage for slope of events
unsigned int totalTimer;//used to calculate period
unsigned int period;//storage for period of wave
byte index = 0;//current storage index
float frequency;//storage for frequency calculations
int maxSlope = 0;//used to calculate max slope as trigger point
int newSlope;//storage for incoming slope data

//variables for decided whether you have a match
byte noMatch = 0;//counts how many non-matches you've received to reset variables if it's been too long
byte slopeTol = 3;//slope tolerance- adjust this if you need
int timerTol = 10;//timer tolerance- adjust this if you need

//variables for amp detection
unsigned int ampTimer = 0;
byte maxAmp = 0;
byte checkMaxAmp;
byte ampThreshold = 30;//raise if you have a very noisy signal
#pragma endregion


void setup()
{
  //Initialize display by providing the display type and its I2C address.
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  //Set the text size and color.
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop()
{
  byte poti_threshold = (analogRead(A0)/4);
  byte poti_ratio = (analogRead(A1)/4);
  float audio_in = analogRead(A2)*(5.0 / 1023.0);
  
  //Clear previous image.
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print(audio_in);

  if (poti_threshold > 0){
    drawCompSettings(poti_threshold,poti_ratio);
  }
  
  display.display();
  delay(30);
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