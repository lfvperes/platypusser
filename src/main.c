#include "raylib.h"
#include "game.h"
#include "character.h"
#include <stdlib.h>

int main() {
    // Initialization
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Platypus Game");
    SetTargetFPS(60);
    int carCount = 0;
    int gatorCount = 0;
    int logCount = 0;
    
    character* cars = NULL;
    character* gators = NULL;
    character* logs = NULL;
    character player;
    initGame(&player);

    // Game loop
    while (!WindowShouldClose()) {
        updateGame(&player, &cars, &carCount, &gators, &gatorCount, &logs, &logCount);
        drawGame(player, cars, carCount, gators, gatorCount, logs, logCount);
    }

    // Cleanup
    free(cars);
    free(gators);
    free(logs);
    CloseWindow();
    return 0;
}