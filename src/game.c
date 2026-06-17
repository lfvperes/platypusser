#include <stdlib.h>
#include "game.h"
#include "street.h"

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
int logCount = 0;

void initGame(character* player) {
    player->position = (Vector2){WINDOW_WIDTH / 2, WINDOW_HEIGHT - PLAYER_SIZE};
    player->velocity = (Vector2){0, 0};
    player->width = PLAYER_SIZE;
    player->height = PLAYER_SIZE;
    player->color = GREEN;
}

void handlePlayerMovement(character* player) {
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

int isSpawnZoneOccupied(character* npcs, int npcCount, int laneY) {
    for (int i = 0; i < npcCount; i++) {
        if (npcs[i].position.y == laneY && npcs[i].position.x < CAR_LENGTH * 2) {
            return 1;
        }
    }
    return 0;
}

character* spawnNPC(character* npcs, int* npcCount, int npcChance, char npcType) {
    if (*npcCount < 10 && GetRandomValue(0, 100) < npcChance) {
        int lane;
        Color spawnColor;
        int minLane, maxLane;
        
        // Determine lane range and color based on npc type
        if (npcType == 'C') {
            minLane = 5;
            maxLane = 8;
            lane = GetRandomValue(minLane, maxLane);
            spawnColor = colors[GetRandomValue(0, MAX_COLORS_COUNT - 1)];
        } else if (npcType == 'G') {
            minLane = 1;
            maxLane = 4;
            lane = GetRandomValue(minLane, maxLane);
            spawnColor = DARKGREEN;
        } else if (npcType = 'L') {
            minLane = 1;
            maxLane = 4;
            lane = GetRandomValue(minLane, maxLane);
            spawnColor = MAROON;
        } else {
            return npcs; // Invalid npc type
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
            case 7: laneY = LANE_7; break;
            case 8: laneY = LANE_8; break;
            default: laneY = LANE_1; break; // Fallback
        }

        // Check if the spawn zone of the lane is occupied
        if (isSpawnZoneOccupied(npcs, *npcCount, laneY)) {
            // Try to find another free lane (only in appropriate range for this npc type)
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
                    case 7: testLaneY = LANE_7; break;
                    case 8: testLaneY = LANE_8; break;
                    default: testLaneY = LANE_1; break; // Fallback
                }
                
                if (!isSpawnZoneOccupied(npcs, *npcCount, testLaneY)) {
                    laneY = testLaneY;
                    foundFreeLane = 1;
                    break;
                }
            }
            if (!foundFreeLane) {
                return npcs; // Skip spawning if all spawn zones are occupied
            }
        }

        // Spawn the npc on the free lane
        npcs = realloc(npcs, (*npcCount + 1) * sizeof(character));
        npcs[*npcCount].position = (Vector2){0, laneY};
        npcs[*npcCount].velocity = (Vector2){CAR_SPEED, 0};
        npcs[*npcCount].width = CAR_LENGTH;
        npcs[*npcCount].height = CAR_HEIGHT;
        npcs[*npcCount].color = spawnColor;
        (*npcCount)++;
    }
    return npcs;
}

void resetGame(character* player, character** cars, int* carCount, character** gators, int* gatorCount, character** logs, int* logCount) {
    // Reset player
    initGame(player);

    // Free and reinitialize cars
    free(*cars);
    *cars = NULL;
    *carCount = 0;
    *gators = NULL;
    *gatorCount = 0;
    *logs = NULL;
    *logCount = 0;

    // Reset game state
    isGameOver = 0;
    screenText = "";
}

void updateGame(character* player, character** cars, int* carCount, character** gators, int* gatorCount, character** logs, int* logCount) {
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
                    *cars = realloc(*cars, *carCount * sizeof(character));
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
                    *gators = realloc(*gators, *gatorCount * sizeof(character));
                } else {
                    free(*gators);
                    *gators = NULL;
                }
            }
        }
        
        // Update log positions and remove out-of-bounds
        for (int i = 0; i < *logCount; i++) {
            (*logs)[i].position.x += (*logs)[i].velocity.x;
            if ((*logs)[i].position.x > WINDOW_WIDTH) {
                // Remove log by shifting remaining logs and reallocating
                for (int j = i; j < *logCount - 1; j++) {
                    (*logs)[j] = (*logs)[j + 1];
                }
                (*logCount)--;
                i--; // Adjust index since we removed an element
                if (*logCount > 0) {
                    *logs = realloc(*logs, *logCount * sizeof(character));
                } else {
                    free(*logs);
                    *logs = NULL;
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
        // check for collisions with gators
        for (int i = 0; i < *gatorCount; i++) {
            if (collision(*player, (*gators)[i])) {
                isGameOver = 1;
                screenText = "GAME OVER - PRESS ENTER TO RESTART";
                break;
            }
        }

        // Spawn new enemies
        *cars = spawnCar(*cars, carCount, CAR_CHANCE);
        *gators = spawnNPC(*gators, gatorCount, CAR_CHANCE, 'G');
        // spawn new logs
        *logs = spawnNPC(*logs, logCount, CAR_CHANCE, 'L');
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
            resetGame(player, cars, carCount, gators, gatorCount, logs, logCount);
        }
    }
}

void drawGame(character player, character* cars, int carCount, character* gators, int gatorCount, character* logs, int logCount) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(screenText, 10, 10, 20, DARKGRAY);

    // draw river
    DrawRectangle(0, RIVER_UPPER_BOUNDARY, WINDOW_WIDTH, RIVER_LOWER_BOUNDARY - RIVER_UPPER_BOUNDARY, SKYBLUE);

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
    // Draw logs
    for (int i = 0; i < logCount; i++) {
        DrawRectangleV(logs[i].position, (Vector2){logs[i].width, logs[i].height}, logs[i].color);
    }
    EndDrawing();
}