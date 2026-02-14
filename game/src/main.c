#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "network.h"
#include "engine.h"

int main(int argc, char* argv[])
{
    int localPlayerId = 0;
    if (!Network_Init(&localPlayerId)) {
        printf("Network Init Failed\n");
        return 1;
    }

    Engine engine;

    if (!engineInit(&engine, localPlayerId))
        return 1;

    engineRun(&engine);
    engineQuit(&engine);
    Network_Shutdown();

    return 0;
}
