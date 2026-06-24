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

    character* hats = NULL;
    
    npcData.carTextures = loadTextures("assets/car", MAX_CAR_TEXTURES);
    npcData.gatorTextures = loadTextures("assets/alligator", 1);

    // lane* riverLane = NULL;
    initGame(&player, &npcData, &hats);

    // Game loop
    while (!WindowShouldClose()) {
        updateGame(&player, &npcData, &hats);
        drawGame(&player, &npcData, &hats);
    }

    // Cleanup
    free(*npcData.cars);
    free(*npcData.gators);
    free(*npcData.logs);
    for (int i = 0; i < MAX_CAR_TEXTURES; i++)
        UnloadTexture(npcData.carTextures[i]);
    UnloadTexture(npcData.gatorTextures[0]);
    free(npcData.carTextures);
    free(npcData.gatorTextures);
    CloseWindow();
    return 0;
}