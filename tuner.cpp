#include "tuner.h"
#include "Arduino.h"

int zaehler;
unsigned long timer;
unsigned long timerOld;
unsigned long startzeit;
unsigned long messzeit = 1000000;



void tuner (){
      if ((micros() - startzeit) >= messzeit)
    {
        float f = timer; //Datentyp 'float', wegen untenstehender Division
        f = 1000000/f; //Aus Periodendauer Frequenz berechnen
        detachInterrupt(0);

            Serial.print("Messung: ");
            Serial.println(f, 1);

        attachInterrupt(digitalPinToInterrupt(2), Messung, RISING);
        zaehler = 0; //Frequenzzähler zurücksetzen
        startzeit = micros(); //Zeitpunkt der letzten Ausgabe speichern
    }
}


void Messung()
{
    zaehler++;
    timer = micros() - timerOld;
    timerOld = micros();
}
