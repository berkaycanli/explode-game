#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

typedef struct {
    bool quit;
    int moveX;
    int moveY;
    bool action;
    bool sprint;
} Input;

void inputInit(Input* input);
void inputUpdate(Input* input, int playerId);

#endif