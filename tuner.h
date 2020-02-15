#ifndef TUNER_H
#define TUNER_H

extern int zaehler;
extern unsigned long timer;
extern unsigned long timerOld;
extern unsigned long startzeit;
extern unsigned long messzeit;

extern void  Messung();
extern float getAvrgFreq(float);

extern char  notes[7];
extern float freq[27][2];

#endif /* TUNER_H */


