#include "types.h"
#include "raylib.h"
#include <stdbool.h>

Timer CreateNewTimer(){
    return (Timer) {
        GetTime(),
        true
    };
}

float GetTimerTime(const Timer *timer) {
    if(!timer->started) return 0;
    return GetTime() - timer->startingTime;    
}

void StopTimer(Timer *timer){
    timer->started = false;
}

void StartTimer(Timer *timer) {
    if(timer->started) return;
    timer->started = true;
    timer->startingTime = GetTime();
}

void RestartTimer(Timer *timer) {
    timer->started = true;
    timer->startingTime = GetTime();
}

int GetAnimationFrame(const Timer *timer, int totalFrames, float frameDuration) {
    float t = GetTimerTime(timer);
    int frame = (int)(t / frameDuration) % totalFrames;
    return frame;
}