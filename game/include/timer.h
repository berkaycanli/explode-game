#ifndef TIMER_H
#define TIMER_H

void timerInit(void);
void timerUpdate(void);
float timerGetDelta(void);
void timerSync(int targetFPS);
float timerGetTotalSeconds(void);

#endif