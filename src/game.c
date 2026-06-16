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
int gatorCount = 0;

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
        if (cars[i].position.y == laneY && cars[i].position.x < CAR_LENGTH * 2) {
            return 1;
        }
    }
    return 0;
}

object* spawnEnemy(object* enemies, int* enemyCount, int enemyChance, char enemyType) {
    if (*enemyCount < 10 && GetRandomValue(0, 100) < enemyChance) {
        int lane;
        Color spawnColor;
        int minLane, maxLane;
        
        // Determine lane range and color based on enemy type
        if (enemyType == 'C') {
            minLane = 4;
            maxLane = 6;
            lane = GetRandomValue(minLane, maxLane);
            spawnColor = colors[GetRandomValue(0, MAX_COLORS_COUNT - 1)];
        } else if (enemyType == 'G') {
            minLane = 1;
            maxLane = 3;
            lane = GetRandomValue(minLane, maxLane);
            spawnColor = DARKGREEN;
        } else {
            return enemies; // Invalid enemy type
        }

        // Map lane number to Y position
        int laneY;
        switch (lane) {
            case 1: laneY = LANE_1; break;
            case 2: laneY = LANE_2; break;
            case 3: laneY = LANE_3; break;
            case 4: laneY = LANE_4; break;
            case 5: laneY = LANE_5; break;
            case 6: laneY = LANE_6; break;
            default: laneY = LANE_1; break; // Fallback
        }

        // Check if the spawn zone of the lane is occupied
        if (isSpawnZoneOccupied(enemies, *enemyCount, laneY)) {
            // Try to find another free lane (only in appropriate range for this enemy type)
            int foundFreeLane = 0;
            for (int i = minLane; i <= maxLane; i++) {
                int testLaneY;
                switch (i) {
                    case 1: testLaneY = LANE_1; break;
                    case 2: testLaneY = LANE_2; break;
                    case 3: testLaneY = LANE_3; break;
                    case 4: testLaneY = LANE_4; break;
                    case 5: testLaneY = LANE_5; break;
                    case 6: testLaneY = LANE_6; break;
                    default: testLaneY = LANE_1; break; // Fallback
                }
                
                if (!isSpawnZoneOccupied(enemies, *enemyCount, testLaneY)) {
                    laneY = testLaneY;
                    foundFreeLane = 1;
                    break;
                }
            }
            if (!foundFreeLane) {
                return enemies; // Skip spawning if all spawn zones are occupied
            }
        }

        // Spawn the enemy on the free lane
        enemies = realloc(enemies, (*enemyCount + 1) * sizeof(object));
        enemies[*enemyCount].position = (Vector2){0, laneY};
        enemies[*enemyCount].velocity = (Vector2){CAR_SPEED, 0};
        enemies[*enemyCount].width = CAR_LENGTH;
        enemies[*enemyCount].height = CAR_HEIGHT;
        enemies[*enemyCount].color = spawnColor;
        (*enemyCount)++;
    }
    return enemies;
}

void resetGame(object* player, object** cars, int* carCount, object** gators, int* gatorCount) {
    // Reset player
    initGame(player);

    // Free and reinitialize cars
    free(*cars);
    *cars = NULL;
    *carCount = 0;
    *gators = NULL;
    *gatorCount = 0;

    // Reset game state
    isGameOver = 0;
    screenText = "";
}

void updateGame(object* player, object** cars, int* carCount, object** gators, int* gatorCount) {
    if (!isGameOver) {
        screenText = "";
        handlePlayerMovement(player);

        // Update car positions and remove out-of-bounds
        for (int i = 0; i < *carCount; i++) {
            (*cars)[i].position.x += (*cars)[i].velocity.x;
            if ((*cars)[i].position.x > WINDOW_WIDTH) {
                // Remove car by shifting remaining cars and reallocating
                for (int j = i; j < *carCount - 1; j++) {
                    (*cars)[j] = (*cars)[j + 1];
                }
                (*carCount)--;
                i--; // Adjust index since we removed an element
                if (*carCount > 0) {
                    *cars = realloc(*cars, *carCount * sizeof(object));
                } else {
                    free(*cars);
                    *cars = NULL;
                }
            }
        }

        // Update gator positions and remove out-of-bounds
        for (int i = 0; i < *gatorCount; i++) {
            (*gators)[i].position.x += (*gators)[i].velocity.x;
            if ((*gators)[i].position.x > WINDOW_WIDTH) {
                // Remove gator by shifting remaining gators and reallocating
                for (int j = i; j < *gatorCount - 1; j++) {
                    (*gators)[j] = (*gators)[j + 1];
                }
                (*gatorCount)--;
                i--; // Adjust index since we removed an element
                if (*gatorCount > 0) {
                    *gators = realloc(*gators, *gatorCount * sizeof(object));
                } else {
                    free(*gators);
                    *gators = NULL;
                }
            }
        }

        // Check for collisions with cars
        for (int i = 0; i < *carCount; i++) {
            if (collision(*player, (*cars)[i])) {
                isGameOver = 1;
                screenText = "GAME OVER - PRESS ENTER TO RESTART";
                break;
            }
        }

        // Spawn new enemies
        *cars = spawnEnemy(*cars, carCount, CAR_CHANCE, 'C');
        *gators = spawnEnemy(*gators, gatorCount, CAR_CHANCE, 'G');
    } else {
        // Stop cars when game is over
        for (int i = 0; i < *carCount; i++) {
            (*cars)[i].velocity.x = 0;
        }
        // Stop gators when game is over
        for (int i = 0; i < *gatorCount; i++) {
            (*gators)[i].velocity.x = 0;
        }

        // Start or restart the game when ENTER is pressed
        if (IsKeyPressed(KEY_ENTER)) {
            resetGame(player, cars, carCount, gators, gatorCount);
        }
    }
}

void drawGame(object player, object* cars, int carCount, object* gators, int gatorCount) {
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
    }
    // Draw gators
    for (int i = 0; i < gatorCount; i++) {
        DrawRectangleV(gators[i].position, (Vector2){gators[i].width, gators[i].height}, gators[i].color);
    }
    EndDrawing();
}