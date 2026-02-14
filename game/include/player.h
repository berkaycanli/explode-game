#ifndef PLAYER_H
#define PLAYER_H

#include "input.h"

typedef struct {
    float x, y, w, h;
} GameRect;

typedef struct {
    float x, y;
    float speed;
    GameRect rect;
    int r, g, b;
} Player;

void playerInit(Player* player, float x, float y, int r, int g, int b);
void playerUpdate(Player* p, Input* input, float dt);

#endif
