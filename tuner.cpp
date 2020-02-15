#include "tuner.h"
#include "Arduino.h"


int zaehler,zAvrg=0;
unsigned long timer, timerOld;
unsigned long startzeit;
unsigned long messzeit = 1000000/4;
float frqColl[5];

//                0        1       2       3       4       5       6       7       8       9       10      11
char notes[12][2] = {{'A',0},{'A',1},{'B',0},{'C',0},{'C',1},{'D',0},{'D',1},{'E',0},{'F',0},{'F',1},{'G',0},{'G',1}};
float freq[61][2] = {
    {27.50,  0 },
    {29.14,  1 },
    {30.87,  2 },
    {32.70,  3 },
    {34.65,  4 },
    {36.71,  5 },
    {38.89,  6 },
    {41.20,  7 },
    {43.65,  8 },
    {46.25,  9 },
    {49.00,  10},
    {51.91,  11},
    {55.00,  0 },
    {58.27,  1 },
    {61.74,  2 },
    {65.41,  3 },
    {69.30,  4 },
    {73.42,  5 },
    {77.78,  6 },
    {82.41,  7 },
    {87.31,  8 },
    {92.50,  9 },
    {98.00,  10},
    {103.83, 11},
    {110.00, 0 },
    {116.54, 1 },
    {123.47, 2 },
    {130.81, 3 },
    {138.59, 4 },
    {146.83, 5 },
    {155.56, 6 },
    {164.81, 7 },
    {174.61, 8 },
    {185.00, 9 },
    {196.00, 10},
    {207.65, 11},
    {220.00, 0 },
    {233.08, 1 },
    {246.94, 2 },
    {261.63, 3 },
    {277.18, 4 },
    {293.66, 5 },
    {311.13, 6 },
    {329.63, 7 },
    {349.23, 8 },
    {369.99, 9 },
    {392.00, 10},
    {415.30, 11},
    {440.00, 0 },
    {466.16, 1 },
    {493.88, 2 },
    {523.25, 3 },
    {554.37, 4 },
    {587.33, 5 },
    {622.25, 6 },
    {659.26, 7 },
    {698.46, 8 },
    {739.99, 9 },
    {783.99, 10},
    {830.61, 11},
    {880.00, 0 }
};


void Messung()
{
    zaehler++;
    timer = micros() - timerOld;
    timerOld = micros();
}


float getAvrgFreq(float fA){
    if(zAvrg>=5)zAvrg=0; //Zähler zurücksetzen
    if(isinf(fA))fA=0; //um fehlerhafte Kalkulation zu vermeiden
    frqColl[zAvrg] = fA;
    zAvrg++;

    float avrg=0.00f;
    for (int i=0; i < 5; i++){
        avrg = avrg + frqColl[i];
    }
    return (avrg/5);
}