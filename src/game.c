#include <stdlib.h>
#include "game.h"
#include "street.h"
#include "river.h"
#include "character.h"

int isGameOver = 1; // Start the game in a "game over" state
char* screenText = "PRESS ENTER TO START";
float hopTimer = 0.0f;
int carCount = 0;
int gatorCount = 0;
int logCount = 0;
lane riverLanes[4];
lane streetLanes[4];
int isPlayerOnLog = 0;

void initGame(character* player) {
    player->position = (Vector2){WINDOW_WIDTH / 2, WINDOW_HEIGHT - PLAYER_SIZE};
    player->velocity = (Vector2){0, 0};
    player->width = PLAYER_SIZE;
    player->height = PLAYER_SIZE;
    player->color = GREEN;

    for (int l = 0; l < 4; l++) {
        int riverLaneSpeed = GetRandomValue(RIVER_MIN_SPEED, RIVER_MAX_SPEED);
        int riverLanePos = LANE_SIZE * (2*l+1);
        riverLanes[l] = (lane){riverLaneSpeed, riverLanePos};

        int streetLaneSpeed = GetRandomValue(RIVER_MIN_SPEED, RIVER_MAX_SPEED);
        int streetLanePos = LANE_SIZE * (10+2*l);
        streetLanes[l] = (lane){streetLaneSpeed, streetLanePos};
    }
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
            if (!isPlayerOnLog)
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
    } else if (isPlayerOnLog) {
        player->position.x += player->velocity.x;
    }

    // Check wall collision
    if (player->position.x < 0) player->position.x = 0;
    if (player->position.y < 0) player->position.y = 0;
    if (player->position.x > WINDOW_WIDTH - player->width) player->position.x = WINDOW_WIDTH - player->width;
    if (player->position.y > WINDOW_HEIGHT - player->height) player->position.y = WINDOW_HEIGHT - player->height;
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

        // Update positions and remove out-of-bounds
        updateNpcPosition(cars, carCount);
        updateNpcPosition(gators, gatorCount);
        updateNpcPosition(logs, logCount);

        // Check for collisions with cars
        for (int i = 0; i < *carCount; i++) {
            if (collision(*player, (*cars)[i])) {
                isGameOver = 1;
                break;
            }
        }
        // Check for collisions with gators
        for (int i = 0; i < *gatorCount; i++) {
            if (collision(*player, (*gators)[i])) {
                isGameOver = 1;
                break;
            }
        }

        // check for collisions with logs
        isPlayerOnLog = 0;
        for (int i = 0; i < *logCount; i++) {
            if (collision(*player, (*logs)[i])) {
                isPlayerOnLog = 1;
                // gets carried by the floating log
                player->velocity.x = (*logs)[i].velocity.x;
                break;
            }
        }
        // check if it's on the river
        if (player->position.y < RIVER_LOWER_BOUNDARY) {
            // check if it's in a river lane
            if (player->position.y == riverLanes[0].yPosition ||
                player->position.y == riverLanes[1].yPosition ||
                player->position.y == riverLanes[2].yPosition ||
                player->position.y == riverLanes[3].yPosition) {
                if (!isPlayerOnLog)
                    isGameOver = 1;
            }
        }

        // Spawn new enemies
        NpcData npcData = {gators, gatorCount, logs, logCount, cars, carCount};  // No dereferencing needed
        *cars = spawnNpc(&npcData, CAR_CHANCE, 'C', streetLanes);
        *gators = spawnNpc(&npcData, CAR_CHANCE, 'G', riverLanes);
        *logs = spawnNpc(&npcData, CAR_CHANCE, 'L', riverLanes);
    } else {
        screenText = "GAME OVER - PRESS ENTER TO RESTART";
        // Stop cars when game is over
        for (int i = 0; i < *carCount; i++) {
            (*cars)[i].velocity.x = 0;
        }
        // Stop gators when game is over
        for (int i = 0; i < *gatorCount; i++) {
            (*gators)[i].velocity.x = 0;
        }
        // Stop logs when game is over
        for (int i = 0; i < *logCount; i++) {
            (*logs)[i].velocity.x = 0;
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
    // Draw player
    DrawRectangleV(player.position, (Vector2){player.width, player.height}, player.color);
    EndDrawing();
}