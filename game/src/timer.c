#include "timer.h"
#include <SDL2/SDL.h>

static unsigned int lastTicks = 0;
static unsigned int frameStartTicks = 0;
static float deltaTime = 0.0f;

void timerInit(void) {
    lastTicks = SDL_GetTicks();
    frameStartTicks = SDL_GetTicks();
}

void timerUpdate(void) {
    unsigned int currentTicks = SDL_GetTicks();
    deltaTime = (currentTicks - lastTicks) / 1000.0f;
    
    if (deltaTime > 0.1f) deltaTime = 0.1f;
    
    lastTicks = currentTicks;
    frameStartTicks = currentTicks;
}

float timerGetDelta(void) {
    return deltaTime;
}

void timerSync(int targetFPS) {
    if (targetFPS <= 0) return;

    unsigned int targetFrameTime = 1000 / targetFPS;
    unsigned int frameTime = SDL_GetTicks() - frameStartTicks;

    if (targetFrameTime > frameTime) {
        SDL_Delay(targetFrameTime - frameTime);
    }
}

float timerGetTotalSeconds(void) {
    return SDL_GetTicks() / 1000.0f;
}
