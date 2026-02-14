#ifndef RENDER_H
#define RENDER_H

#include <stdbool.h>

bool renderInit(const char* title, int width, int height);
void renderBegin(void);
void renderDrawRect(float x, float y, float w, float h, int r, int g, int b, int a);
void renderEnd(void);
void renderQuit(void);

#endif