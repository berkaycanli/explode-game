#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>
#include "input.h"
#include "player.h"

#include "config.h"


typedef struct {
    bool running;
    int localPlayerId;
    Player players[MAX_PLAYERS];
    Input inputs[MAX_PLAYERS];
} Engine;

bool engineInit(Engine* engine, int localPlayerId);
void engineRun(Engine* engine);
void engineQuit(Engine* engine);

#endif