#ifndef TIMERS_H
#define TIMERS_H

#include "types.h"

Timer CreateNewTimer();
float GetTimerTime(const Timer *timer);
void StopTimer(Timer *timer);
void StartTimer(Timer *timer);
void RestartTimer(Timer *timer);
int GetAnimationFrame(const Timer *timer, int totalFrames, float frameDuration);

#endif