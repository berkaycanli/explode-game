#include "input.h"
#include "config.h"
#include <SDL2/SDL.h>

void inputInit(Input* input) {
    input->quit = false;
    input->moveX = 0;
    input->moveY = 0;
    input->action = false;
}

void inputUpdate(Input* input, int playerId) {
    SDL_Event e;
    const Uint8* kb = SDL_GetKeyboardState(NULL);

    input->moveX = 0;
    input->moveY = 0;
    input->action = false;
    input->sprint = false;

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) input->quit = true;
    }

    if (kb[KEY_EXIT]) input->quit = true;

    if (playerId == 0) {
        if (kb[KEY_P1_RIGHT]) input->moveX = 1;
        if (kb[KEY_P1_LEFT])  input->moveX = -1;
        if (kb[KEY_P1_UP])    input->moveY = -1;
        if (kb[KEY_P1_DOWN])  input->moveY = 1;
        if (kb[KEY_P1_ACTION]) input->action = true;
        if (kb[KEY_P1_SPRINT]) input->sprint = true;
    } else if (playerId == 1) {
        if (kb[KEY_P2_RIGHT]) input->moveX = 1;
        if (kb[KEY_P2_LEFT])  input->moveX = -1;
        if (kb[KEY_P2_UP])    input->moveY = -1;
        if (kb[KEY_P2_DOWN])  input->moveY = 1;
        if (kb[KEY_P2_ACTION]) input->action = true;
        if (kb[KEY_P2_SPRINT]) input->sprint = true;
    }
}
