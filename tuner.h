#ifndef TUNER_H
#define TUNER_H

extern int zaehler;
extern unsigned long timer;
extern unsigned long timerOld;
extern unsigned long startzeit;
extern unsigned long messzeit;

extern void  Messung();
extern float getAvrgFreq(float);
extern float getEdgeL(int);
extern float getEdgeH(int);
extern int   getFreqArrSize();
extern char  getNote(int);
extern float getRefFreq(int);


#endif /* TUNER_H */


