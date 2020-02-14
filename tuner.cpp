#include "tuner.h"
#include "Arduino.h"

int zaehler;
unsigned long timer;
unsigned long timerOld;
unsigned long startzeit;
unsigned long messzeit = 1000000;



void tuner (){

}


void Messung()
{
    zaehler++;
    timer = micros() - timerOld;
    timerOld = micros();
}
