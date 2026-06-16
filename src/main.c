#include "raylib.h"
#include "game.h"
#include "object.h"
#include <stdlib.h>

int main() {
    // Initialization
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Platypus Game");
    SetTargetFPS(60);
    int carCount = 0;
    
    object* cars = NULL;
    object player;
    initGame(&player);

    // Game loop
    while (!WindowShouldClose()) {
        updateGame(&player, &cars, &carCount);
        drawGame(player, cars, carCount);
    }

    // Cleanup
    free(cars);
    CloseWindow();
    return 0;
}