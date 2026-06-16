#include "raylib.h"
#include "game.h"
#include "object.h"
#include <stdlib.h>

int main() {
    // Initialization
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Platypus Game");
    SetTargetFPS(60);
    int carCount = 0;
    int gatorCount = 0;
    
    object* cars = NULL;
    object* gators = NULL;
    object player;
    initGame(&player);

    // Game loop
    while (!WindowShouldClose()) {
        updateGame(&player, &cars, &carCount, &gators, &gatorCount);
        drawGame(player, cars, carCount, gators, gatorCount);
    }

    // Cleanup
    free(cars);
    free(gators);
    CloseWindow();
    return 0;
}