#ifndef CONFIG_H
#define CONFIG_H

// Ekran Ayarları
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define WINDOW_TITLE  "Solveritius Game Engine"

#define FPS_TARGET 144
// 1000 milisaniye / 60 kare = Her kare yaklaşık 16.6 ms sürmeli
#define FRAME_TARGET_TIME (1000 / FPS_TARGET)

// Fizik Ayarları
#define MAX_DELTA_TIME 0.05f


#define MAX_PLAYERS 2

// --- TUS ATAMALARI ---
// Oyuncu 1
#define KEY_P1_UP      SDL_SCANCODE_W
#define KEY_P1_DOWN    SDL_SCANCODE_S
#define KEY_P1_LEFT    SDL_SCANCODE_A
#define KEY_P1_RIGHT   SDL_SCANCODE_D
#define KEY_P1_SPRINT  SDL_SCANCODE_LCTRL
#define KEY_P1_ACTION  SDL_SCANCODE_SPACE

// Oyuncu 2
#define KEY_P2_UP      SDL_SCANCODE_UP
#define KEY_P2_DOWN    SDL_SCANCODE_DOWN
#define KEY_P2_LEFT    SDL_SCANCODE_LEFT
#define KEY_P2_RIGHT   SDL_SCANCODE_RIGHT
#define KEY_P2_SPRINT  SDL_SCANCODE_RCTRL
#define KEY_P2_ACTION  SDL_SCANCODE_RSHIFT

#define KEY_EXIT    SDL_SCANCODE_ESCAPE


#endif