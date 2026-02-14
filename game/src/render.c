#include "render.h"
#include <SDL2/SDL.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

bool renderInit(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    return true;
}

void renderBegin(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void renderDrawRect(float x, float y, float w, float h, int r, int g, int b, int a) {
    SDL_Rect rect = { (int)x, (int)y, (int)w, (int)h };
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &rect);
}

void renderEnd(void) {
    SDL_RenderPresent(renderer);
}

void renderQuit(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

