#include "tuner.h"
#include "Arduino.h"

int zaehler;
unsigned long timer;
unsigned long timerOld;
unsigned long startzeit;
unsigned long messzeit = 1000000/4;

//                0   1   2   3   4   5   6
char notes[7] = {'C','D','E','F','G','A','B'};
float freq[27][2] = {
    {27.5, 5},
    {30.9, 6},
    {32.7, 0},
    {36.7, 1},
    {41.2, 2},
    {43.7, 3},
    {49.0, 4},
    {55.0, 5},
    {61.7, 6},
    {65.4, 0},
    {73.4, 1},
    {82.4, 2},
    {87.3, 3},
    {98.0, 4},
    {110.0,5},
    {123.4,6},
    {130.8,0},
    {146.8,1},
    {164.8,2},
    {174.6,3},
    {196.0,4},
    {220.0,5},
    {246.9,6},
    {261.6,0},
    {293.7,1},
    {329.6,2},
    {349.2,3}
};


void Messung()
{
    zaehler++;
    timer = micros() - timerOld;
    timerOld = micros();
}
