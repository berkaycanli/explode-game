#include "player.h"
#include "config.h"

void playerInit(Player* player, float x, float y, int r, int g, int b) {
    player->x = x;
    player->y = y;
    player->speed = 300.0f;
    player->rect.w = 50;
    player->rect.h = 50;
    player->r = r;
    player->g = g;
    player->b = b;
}

void playerUpdate(Player* p, Input* input, float dt) {
    float currentSpeed = p->speed;
    if (input->sprint) {
        currentSpeed *= 1.8f; // Sprint çarpanı
    }

    float nextX = p->x + (input->moveX * currentSpeed *dt);
    float nextY = p->y + (input->moveY * currentSpeed * dt);

    if (nextX >= 0 && nextX <= (SCREEN_WIDTH - p->rect.w)) {
        p->x = nextX;
    }
    if (nextY >= 0 && nextY <= (SCREEN_HEIGHT - p->rect.h)) {
        p->y = nextY;
    }

    p->rect.x = p->x;
    p->rect.y = p->y;
}

