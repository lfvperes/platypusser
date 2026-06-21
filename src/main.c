#include "raylib.h"
#include "game.h"
#include "character.h"
#include <stdlib.h>

int main() {
    // Initialization
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Platypus Game");
    SetTargetFPS(60);

    character player;
    NpcData npcData;
    
    character* cars = NULL;
    npcData.cars = &cars;
    int carCount = 0;
    npcData.carCount = &carCount;
    
    character* gators = NULL;
    npcData.gators = &gators;
    int gatorCount = 0;
    npcData.gatorCount = &gatorCount;
    
    character* logs = NULL;
    npcData.logs = &logs;
    int logCount = 0;
    npcData.logCount = &logCount;
    
    // lane* riverLane = NULL;
    initGame(&player, &npcData);

    // Game loop
    while (!WindowShouldClose()) {
        updateGame(&player, &npcData);
        drawGame(&player, &npcData);
    }

    // Cleanup
    free(*npcData.cars);
    free(*npcData.gators);
    free(*npcData.logs);
    CloseWindow();
    return 0;
}