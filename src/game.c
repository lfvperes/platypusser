#include <stdlib.h>
#include "game.h"
#include "street.h"
#include "river.h"
#include "character.h"

int isGameOver = 1; // Start the game in a "game over" state
char* topText = "PRESS ENTER TO START";
char* centerText = "";
float hopTimer = 0.0f;
lane riverLanes[RIVER_LANE_COUNT];
lane streetLanes[STREET_LANE_COUNT];
int isPlayerOnLog = 0;

void initGame(character* player, NpcData* npcData, character** hats) {
    player->position = (Vector2){WINDOW_WIDTH / 2, WINDOW_HEIGHT - PLAYER_SIZE};
    player->velocity = (Vector2){0, 0};
    player->width = PLAYER_SIZE;
    player->height = PLAYER_SIZE;
    player->color = GREEN;

    for (int l = 0; l < RIVER_LANE_COUNT; l++) {
        int riverLaneSpeed = GetRandomValue(RIVER_MIN_SPEED, RIVER_MAX_SPEED);
        int riverLanePos = RIVER_UPPER_BOUNDARY + LANE_SIZE * l;
        riverLanes[l] = (lane){riverLaneSpeed, riverLanePos};
    }
    for (int l = 0; l < STREET_LANE_COUNT; l++) {
        int streetLaneSpeed = GetRandomValue(STREET_MIN_SPEED, STREET_MAX_SPEED);
        int streetLanePos = STREET_UPPER_BOUNDARY + LANE_SIZE * l;
        streetLanes[l] = (lane){streetLaneSpeed, streetLanePos};
    }

    // Initialize NpcData
    *npcData->gators = NULL;
    *npcData->gatorCount = 0;
    *npcData->logs = NULL;
    *npcData->logCount = 0;
    *npcData->cars = NULL;
    *npcData->carCount = 0;

    // initialize hats (goals)
    // Allocate memory for hats (e.g., 5 hats)
    *hats = malloc(HAT_COUNT * sizeof(character));
    for (int i = 0; i < HAT_COUNT; i++) {
        (*hats)[i].position = (Vector2){4*BLOCK_SIZE + 4*i * BLOCK_SIZE, BLOCK_SIZE}; // Set positions for hats
        (*hats)[i].width = BLOCK_SIZE;
        (*hats)[i].height = BLOCK_SIZE;
        (*hats)[i].color = MAROON;
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

void resetGame(character* player, NpcData* npcData, character** hats) {
    free(*npcData->cars);
    free(*npcData->gators);
    free(*npcData->logs);

    // Reset player
    initGame(player, npcData, hats);

    // Reset game state
    isGameOver = 0;
    centerText = "";
}

void updateGame(character* player, NpcData* npcData, character** hats) {
    if (!isGameOver) {
        topText = "";
        handlePlayerMovement(player);

        // Update positions and remove out-of-bounds
        updateNpcPosition(npcData->cars, npcData->carCount);
        updateNpcPosition(npcData->gators, npcData->gatorCount);
        updateNpcPosition(npcData->logs, npcData->logCount);

        // Check for collisions with cars
        for (int i = 0; i < *npcData->carCount; i++) {
            if (collision(*player, (*npcData->cars)[i])) {
                isGameOver = 1;
                centerText = "GAME OVER";
                break;
            }
        }
        // Check for collisions with gators
        for (int i = 0; i < *npcData->gatorCount; i++) {
            if (collision(*player, (*npcData->gators)[i])) {
                isGameOver = 1;
                centerText = "GAME OVER";
                break;
            }
        }

        // check for collisions with logs
        isPlayerOnLog = 0;
        for (int i = 0; i < *npcData->logCount; i++) {
            if (collision(*player, (*npcData->logs)[i])) {
                isPlayerOnLog = 1;
                // gets carried by the floating log
                player->velocity.x = (*npcData->logs)[i].velocity.x;
                break;
            }
        }
        // check if it's on the river
        if (player->position.y < RIVER_LOWER_BOUNDARY && player->position.y > RIVER_UPPER_BOUNDARY) {
            if (!isPlayerOnLog) {
                isGameOver = 1;
                centerText = "GAME OVER";
            }
        }
        // check if crossed the river
        if (player->position.y < RIVER_UPPER_BOUNDARY) {
            int hatCollided = 0;
            for (int h = 0; h < HAT_COUNT; h++) {
                if (collision(*player, (*hats)[h])) {
                    centerText = "BONUS WIN!";
                    hatCollided = 1;
                    break;
                }
            }
            if (!hatCollided)
                centerText = "YOU WIN";
            isGameOver = 1;
        }

        // Spawn new NPCs
        *npcData->cars = spawnNpc(npcData, CAR_CHANCE, 'C', streetLanes);
        *npcData->gators = spawnNpc(npcData, GATOR_CHANCE, 'G', riverLanes);
        *npcData->logs = spawnNpc(npcData, LOG_CHANCE, 'L', riverLanes);
    } else {
        topText = "PRESS ENTER TO RESTART";
        // Stop cars when game is over
        for (int i = 0; i < *npcData->carCount; i++) {
            (*npcData->cars)[i].velocity.x = 0;
        }
        // Stop gators when game is over
        for (int i = 0; i < *npcData->gatorCount; i++) {
            (*npcData->gators)[i].velocity.x = 0;
        }
        // Stop logs when game is over
        for (int i = 0; i < *npcData->logCount; i++) {
            (*npcData->logs)[i].velocity.x = 0;
        }

        // Start or restart the game when ENTER is pressed
        if (IsKeyPressed(KEY_ENTER)) {
            resetGame(player, npcData, hats);
        }
    }
}

void drawGame(character* player, NpcData* npcData, character** hats, Texture2D playerTexture, Texture2D hatTexture) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    // draw river
    DrawRectangle(0, RIVER_UPPER_BOUNDARY, WINDOW_WIDTH, RIVER_LOWER_BOUNDARY - RIVER_UPPER_BOUNDARY, SKYBLUE);
    
    // Draw street
    DrawRectangle(0, STREET_UPPER_BOUNDARY, WINDOW_WIDTH, STREET_LOWER_BOUNDARY - STREET_UPPER_BOUNDARY, DARKGRAY);
    
    // Draw cars
    for (int i = 0; i < *npcData->carCount; i++) {
        Texture2D tex = npcData->carTextures[(*npcData->cars)[i].textureIndex];
        DrawTexturePro(
            tex,
            (Rectangle){0, 0, tex.width, tex.height},
            (Rectangle){(*npcData->cars)[i].position.x, (*npcData->cars)[i].position.y, (*npcData->cars)[i].width, (*npcData->cars)[i].height}, // dest: where on screen
            (Vector2){0, 0}, // origin for rotation
            0.0f,            // rotation
            WHITE  
        );
    }
    // Draw gators
    for (int i = 0; i < *npcData->gatorCount; i++) {
        Texture2D tex = npcData->gatorTextures[(*npcData->gators)[i].textureIndex];
        DrawTexturePro(
            tex,
            (Rectangle){0, 0, tex.width, tex.height},
            (Rectangle){(*npcData->gators)[i].position.x, (*npcData->gators)[i].position.y, (*npcData->gators)[i].width, (*npcData->gators)[i].height},
            (Vector2){0, 0}, // origin for rotation
            0.0f,            // rotation
            WHITE  
        );
    }
    // Draw logs
    for (int i = 0; i < *npcData->logCount; i++) {
        Texture2D tex = npcData->logTextures[(*npcData->logs)[i].textureIndex];
        DrawTexturePro(
            tex,
            (Rectangle){0, 0, tex.width, tex.height},
            (Rectangle){(*npcData->logs)[i].position.x, (*npcData->logs)[i].position.y, (*npcData->logs)[i].width, (*npcData->logs)[i].height},
            (Vector2){0, 0}, // origin for rotation
            0.0f,            // rotation
            WHITE  
        );
    }
    // draw hats
    for (int i = 0; i < HAT_COUNT; i++) {
        // DrawCircleV((*hats)[i].position, (*hats)[i].width/2, (*hats)[i].color);
        DrawTexturePro(
            hatTexture,
            (Rectangle){0, 0, hatTexture.width, hatTexture.height},
            (Rectangle){(*hats)[i].position.x, (*hats)[i].position.y, (*hats)[i].width, (*hats)[i].height},
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
    }
    
    // Draw player
    DrawRectangleV(player->position, (Vector2){player->width, player->height}, player->color);
    
    // draw text
    DrawText(topText, 10, 10, 20, DARKGRAY);
    DrawText(centerText, WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 40, GREEN);
    
    EndDrawing();
}