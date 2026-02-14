#include "engine.h"
#include "render.h"
#include "timer.h"
#include "config.h"
#include "network.h"

bool engineInit(Engine* engine, int localPlayerId) {
    if (!renderInit(WINDOW_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT))
        return false;

    timerInit();

    // Player 1 Initialization (Left Side, White)
    playerInit(&engine->players[0], 
        (float)SCREEN_WIDTH * 0.25f - 25, 
        (float)SCREEN_HEIGHT / 2 - 25, 
        255, 255, 255);
    inputInit(&engine->inputs[0]);

    // Player 2 Initialization (Right Side, Blue)
    playerInit(&engine->players[1], 
        (float)SCREEN_WIDTH * 0.75f - 25, 
        (float)SCREEN_HEIGHT / 2 - 25, 
        50, 150, 255);
    inputInit(&engine->inputs[1]);

    engine->localPlayerId = localPlayerId;
    engine->running = true;
    return true;
}

void engineRun(Engine* engine) {


    while (engine->running) {
        timerUpdate();
        float dt = timerGetDelta();


        // Update Inputs
        for (int i = 0; i < MAX_PLAYERS; i++) {
            inputUpdate(&engine->inputs[i], i);
        }

        // Synchronize Inputs over Network
        // We only care about sending OUR input and receiving THEIR input.
        Network_Update(&engine->inputs[0], &engine->inputs[1], engine->localPlayerId);


        // Update Logic
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (engine->inputs[i].quit) engine->running = false;
            
            playerUpdate(&engine->players[i], &engine->inputs[i], dt);
        }

        renderBegin();
        
        // Render Loop
        for (int i = 0; i < MAX_PLAYERS; i++) {
             renderDrawRect(
                engine->players[i].x, 
                engine->players[i].y, 
                engine->players[i].rect.w, 
                engine->players[i].rect.h, 
                engine->players[i].r, 
                engine->players[i].g, 
                engine->players[i].b, 
                255
            );
        }
        
        renderEnd();

        timerSync(FPS_TARGET);
    }
}

void engineQuit(Engine* engine) {
    renderQuit();
}