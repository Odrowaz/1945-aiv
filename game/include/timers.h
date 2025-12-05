#ifndef TIMERS_H
#define TIMERS_H

#include "types.h"

Timer_t CreateNewTimer();
float CycleTimerTime(Timer_t *timer);
float GetTimerTime(const Timer_t *timer);
void StopTimer(Timer_t *timer);
void StartTimer(Timer_t *timer);
void RestartTimer(Timer_t *timer);
int GetAnimationFrame(const Timer_t *timer, int totalFrames, float frameDuration);

#endif