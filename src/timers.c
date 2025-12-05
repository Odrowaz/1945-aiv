#include "raylib.h"
#include "types.h"
#include <stdbool.h>

Timer_t CreateNewTimer() {
  return (Timer_t){
      GetTime(),
      true};
}

float CycleTimerTime(Timer_t *timer) {
  if (!timer->started) return 0;
  float time = GetTime() - timer->startingTime;
  timer->startingTime = GetTime();
  return time;
}

float GetTimerTime(const Timer_t *timer) {
  if (!timer->started) return 0;
  return GetTime() - timer->startingTime;
}

void StopTimer(Timer_t *timer) {
  timer->started = false;
}

void StartTimer(Timer_t *timer) {
  if (timer->started) return;
  timer->started = true;
  timer->startingTime = GetTime();
}

void RestartTimer(Timer_t *timer) {
  timer->started = true;
  timer->startingTime = GetTime();
}

int GetAnimationFrame(const Timer_t *timer, int totalFrames, float frameDuration) {
  float t = GetTimerTime(timer);
  int frame = (int)(t / frameDuration) % totalFrames;
  return frame;
}