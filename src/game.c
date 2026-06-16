#include "game.h"
#include <stdlib.h>

Color colors[MAX_COLORS_COUNT] = {
    DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
    GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
    GREEN, SKYBLUE, PURPLE, BEIGE
};

int isGameOver = 1; // Start the game in a "game over" state
char* screenText = "PRESS ENTER TO START";
float hopTimer = 0.0f;
int carCount = 0;

void initGame(object* player) {
    player->position = (Vector2){WINDOW_WIDTH / 2, WINDOW_HEIGHT - PLAYER_SIZE};
    player->velocity = (Vector2){0, 0};
    player->width = PLAYER_SIZE;
    player->height = PLAYER_SIZE;
    player->color = GREEN;
}

void handlePlayerMovement(object* player) {
    hopTimer -= GetFrameTime();
    if (hopTimer <= 0.0f) {
        if (IsKeyDown(KEY_RIGHT)) {
            player->velocity.x = PLAYER_STEP;
            player->velocity.y = 0;
        } else if (IsKeyDown(KEY_LEFT)) {
            player->velocity.x = -PLAYER_STEP;
            player->velocity.y = 0;
        } else {
            player->velocity.x = 0;
        }
        if (IsKeyDown(KEY_DOWN)) {
            player->velocity.y = PLAYER_STEP;
            player->velocity.x = 0;
        } else if (IsKeyDown(KEY_UP)) {
            player->velocity.y = -PLAYER_STEP;
            player->velocity.x = 0;
        } else {
            player->velocity.y = 0;
        }

        player->position.x += player->velocity.x;
        player->position.y += player->velocity.y;
        hopTimer = DELAY;
    }

    // Check wall collision
    if (player->position.x < 0) player->position.x = 0;
    if (player->position.y < 0) player->position.y = 0;
    if (player->position.x > WINDOW_WIDTH - player->width) player->position.x = WINDOW_WIDTH - player->width;
    if (player->position.y > WINDOW_HEIGHT - player->height) player->position.y = WINDOW_HEIGHT - player->height;
}

int isSpawnZoneOccupied(object* cars, int carCount, int laneY) {
    for (int i = 0; i < carCount; i++) {
        if (cars[i].position.y == laneY && cars[i].position.x < CAR_LENGTH) {
            return 1;
        }
    }
    return 0;
}

object* spawnCar(object* cars, int* carCount, int carChance) {
    if (*carCount < 10 && GetRandomValue(0, 100) < carChance) {
        int lane = GetRandomValue(1, 3);
        int laneY;

        // Map lane number to Y position
        switch (lane) {
            case 1: laneY = LANE_1; break;
            case 2: laneY = LANE_2; break;
            case 3: laneY = LANE_3; break;
        }

        // Check if the spawn zone of the lane is occupied
        if (isSpawnZoneOccupied(cars, *carCount, laneY)) {
            // Try to find another free lane
            int foundFreeLane = 0;
            for (int i = 1; i <= 3; i++) {
                int testLaneY;
                switch (i) {
                    case 1: testLaneY = LANE_1; break;
                    case 2: testLaneY = LANE_2; break;
                    case 3: testLaneY = LANE_3; break;
                }
                if (!isSpawnZoneOccupied(cars, *carCount, testLaneY)) {
                    laneY = testLaneY;
                    foundFreeLane = 1;
                    break;
                }
            }
            if (!foundFreeLane) {
                return cars; // Skip spawning if all spawn zones are occupied
            }
        }

        // Spawn the car on the free lane
        cars = realloc(cars, (*carCount + 1) * sizeof(object));
        cars[*carCount].position = (Vector2){0, laneY};
        cars[*carCount].velocity = (Vector2){CAR_SPEED, 0};
        cars[*carCount].width = CAR_LENGTH;
        cars[*carCount].height = CAR_HEIGHT;
        cars[*carCount].color = colors[GetRandomValue(0, MAX_COLORS_COUNT - 1)];
        (*carCount)++;
    }
    return cars;
}

void resetGame(object* player, object** cars, int* carCount) {
    // Reset player
    initGame(player);

    // Free and reinitialize cars
    free(*cars);
    *cars = NULL;
    *carCount = 0;

    // Reset game state
    isGameOver = 0;
    screenText = "";
}

void updateGame(object* player, object** cars, int* carCount) {
    if (!isGameOver) {
        screenText = "";
        handlePlayerMovement(player);

        // Check for collisions with cars
        for (int i = 0; i < *carCount; i++) {
            if (collision(*player, (*cars)[i])) {
                isGameOver = 1;
                screenText = "GAME OVER - PRESS ENTER TO RESTART";
                break;
            }
        }

        // Spawn new cars
        *cars = spawnCar(*cars, carCount, CAR_CHANCE);
    } else {
        // Stop cars when game is over
        for (int i = 0; i < *carCount; i++) {
            (*cars)[i].velocity.x = 0;
        }

        // Start or restart the game when ENTER is pressed
        if (IsKeyPressed(KEY_ENTER)) {
            resetGame(player, cars, carCount);
        }
    }
}

void drawGame(object player, object* cars, int carCount) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(screenText, 10, 10, 20, DARKGRAY);

    // Draw street
    DrawRectangle(0, STREET_UPPER_BOUNDARY, WINDOW_WIDTH, STREET_LOWER_BOUNDARY - STREET_UPPER_BOUNDARY, DARKGRAY);
    
    // Draw player
    DrawRectangleV(player.position, (Vector2){player.width, player.height}, player.color);

    // Draw cars
    for (int i = 0; i < carCount; i++) {
        DrawRectangleV(cars[i].position, (Vector2){cars[i].width, cars[i].height}, cars[i].color);
        cars[i].position.x += cars[i].velocity.x;
        if (cars[i].position.x > WINDOW_WIDTH) {
            cars[i].position.x = 0;
        }
    }
    EndDrawing();
}