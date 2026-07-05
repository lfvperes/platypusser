#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "street.h"
#include "river.h"
#include "character.h"
#include "scores.h"

int isGameOver = 1; // Começa em "game over" para mostrar a tela inicial
char* topText    = "PRESSIONE ENTER PARA COMECAR";
char* centerText = "";
float hopTimer   = 0.0f;
lane riverLanes[RIVER_LANE_COUNT];
lane streetLanes[STREET_LANE_COUNT];
int isPlayerOnLog = 0;

float elapsedTime  = 0.0f;
int isEnteringName = 0;
char playerName[4] = "AAA";
int nameCharIndex  = 0;

Score scores[MAX_SCORES];
int scoreCount = 0;
Stats stats;

void initGame(character* player, NpcData* npcData, character** hats) {
    player->position = (Vector2){WINDOW_WIDTH / 2, WINDOW_HEIGHT - PLAYER_SIZE};
    player->velocity = (Vector2){0, 0};
    player->width    = 2 * PLAYER_SIZE;
    player->height   = PLAYER_SIZE;
    player->color    = GREEN;

    for (int l = 0; l < RIVER_LANE_COUNT; l++) {
        int riverLaneSpeed = GetRandomValue(RIVER_MIN_SPEED, RIVER_MAX_SPEED);
        int riverLanePos   = RIVER_UPPER_BOUNDARY + LANE_SIZE * l;
        riverLanes[l] = (lane){riverLaneSpeed, riverLanePos};
    }
    for (int l = 0; l < STREET_LANE_COUNT; l++) {
        int streetLaneSpeed = GetRandomValue(STREET_MIN_SPEED, STREET_MAX_SPEED);
        int streetLanePos   = STREET_UPPER_BOUNDARY + LANE_SIZE * l;
        streetLanes[l] = (lane){streetLaneSpeed, streetLanePos};
    }

    // Initialize NpcData
    *npcData->gators     = NULL;
    *npcData->gatorCount = 0;
    *npcData->logs       = NULL;
    *npcData->logCount   = 0;
    *npcData->cars       = NULL;
    *npcData->carCount   = 0;

    // Initialize hats (goals)
    *hats = malloc(HAT_COUNT * sizeof(character));
    for (int i = 0; i < HAT_COUNT; i++) {
        (*hats)[i].position = (Vector2){4*BLOCK_SIZE + 4*i * BLOCK_SIZE, BLOCK_SIZE};
        (*hats)[i].width    = BLOCK_SIZE;
        (*hats)[i].height   = BLOCK_SIZE;
        (*hats)[i].color    = MAROON;
    }

    // Load scores and stats from files
    scoreCount = loadScores(scores);
    loadStats(&stats);
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
    if (player->position.x > WINDOW_WIDTH  - player->width)  player->position.x = WINDOW_WIDTH  - player->width;
    if (player->position.y > WINDOW_HEIGHT - player->height) player->position.y = WINDOW_HEIGHT - player->height;
}

void resetGame(character* player, NpcData* npcData, character** hats) {
    free(*npcData->cars);
    free(*npcData->gators);
    free(*npcData->logs);

    // Reset player and lanes
    initGame(player, npcData, hats);

    // Reset game state
    isGameOver     = 0;
    centerText     = "";
    elapsedTime    = 0.0f;
    isEnteringName = 0;
    strncpy(playerName, "AAA", 4);
    nameCharIndex  = 0;
}

// Handles 3-letter name input after a win; up/down cycle the current letter, right/enter advances or confirms
static void handleNameEntry() {
    if (IsKeyPressed(KEY_UP)) {
        playerName[nameCharIndex]++;
        if (playerName[nameCharIndex] > 'Z') playerName[nameCharIndex] = 'A';
    }
    if (IsKeyPressed(KEY_DOWN)) {
        playerName[nameCharIndex]--;
        if (playerName[nameCharIndex] < 'A') playerName[nameCharIndex] = 'Z';
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_ENTER)) {
        if (nameCharIndex < 2) {
            nameCharIndex++;
        } else {
            // Save score and stats on confirm
            insertScore(scores, &scoreCount, playerName, elapsedTime);
            saveScores(scores, scoreCount);
            saveStats(&stats);
            isEnteringName = 0;
            topText = "PRESSIONE ENTER PARA REINICIAR";
        }
    }
    if (IsKeyPressed(KEY_LEFT) && nameCharIndex > 0) {
        nameCharIndex--;
    }
}

void updateGame(character* player, NpcData* npcData, character** hats) {
    // Name entry state after a win
    if (isEnteringName) {
        handleNameEntry();
        return;
    }

    if (!isGameOver) {
        topText = "";
        elapsedTime += GetFrameTime();

        handlePlayerMovement(player);

        // Update positions and remove out-of-bounds
        updateNpcPosition(npcData->cars,   npcData->carCount);
        updateNpcPosition(npcData->gators, npcData->gatorCount);
        updateNpcPosition(npcData->logs,   npcData->logCount);

        // Check for collisions with cars
        for (int i = 0; i < *npcData->carCount; i++) {
            if (collision(*player, (*npcData->cars)[i])) {
                isGameOver = 1;
                centerText = "GAME OVER";
                stats.losses++;
                saveStats(&stats);
                break;
            }
        }
        // Check for collisions with gators
        for (int i = 0; i < *npcData->gatorCount; i++) {
            if (collision(*player, (*npcData->gators)[i])) {
                isGameOver = 1;
                centerText = "GAME OVER";
                stats.losses++;
                saveStats(&stats);
                break;
            }
        }

        // Check for collisions with logs
        isPlayerOnLog = 0;
        for (int i = 0; i < *npcData->logCount; i++) {
            if (collision(*player, (*npcData->logs)[i])) {
                isPlayerOnLog = 1;
                // Gets carried by the floating log
                player->velocity.x = (*npcData->logs)[i].velocity.x;
                break;
            }
        }
        // Check if player is on the river without a log
        if (player->position.y < RIVER_LOWER_BOUNDARY && player->position.y > RIVER_UPPER_BOUNDARY) {
            if (!isPlayerOnLog) {
                isGameOver = 1;
                centerText = "GAME OVER";
                stats.losses++;
                saveStats(&stats);
            }
        }
        // Check if player crossed the river (win)
        if (player->position.y < RIVER_UPPER_BOUNDARY) {
            int hatCollided = 0;
            for (int h = 0; h < HAT_COUNT; h++) {
                if (collision(*player, (*hats)[h])) {
                    centerText = "BONUS WIN!";
                    hatCollided = 1;
                    break;
                }
            }
            if (!hatCollided) centerText = "YOU WIN";

            isGameOver = 1;
            stats.wins++;
            // Don't save stats yet: wait for the player to enter their name
            isEnteringName = 1;
            strncpy(playerName, "AAA", 4);
            nameCharIndex = 0;
            topText = "";
        }

        // Spawn new NPCs
        *npcData->cars   = spawnNpc(npcData, CAR_CHANCE,   'C', streetLanes);
        *npcData->gators = spawnNpc(npcData, GATOR_CHANCE, 'G', riverLanes);
        *npcData->logs   = spawnNpc(npcData, LOG_CHANCE,   'L', riverLanes);

    } else {
        topText = "PRESSIONE ENTER PARA REINICIAR";

        // Stop NPCs while game is paused
        for (int i = 0; i < *npcData->carCount; i++)   (*npcData->cars)[i].velocity.x   = 0;
        for (int i = 0; i < *npcData->gatorCount; i++) (*npcData->gators)[i].velocity.x = 0;
        for (int i = 0; i < *npcData->logCount; i++)   (*npcData->logs)[i].velocity.x   = 0;

        if (IsKeyPressed(KEY_ENTER)) {
            resetGame(player, npcData, hats);
        }
    }
}

// Draws top 5 scores and win/loss stats on the game over / start screen
static void drawScoreboard() {
    int x = 20;
    int y = 50;

    // Background panel: title (25) + up to MAX_SCORES lines (20 each) + gap (10) + stats line (20) + padding (20)
    DrawRectangle(x - 5, y - 5, 340, 25 + MAX_SCORES * 20 + 10 + 20 + 20, RAYWHITE);

    DrawText("TOP 5 - MELHORES TEMPOS", x, y, 18, DARKBLUE);
    y += 25;

    if (scoreCount == 0) {
        DrawText("Nenhum recorde ainda.", x, y, 16, GRAY);
        y += 20;
    } else {
        for (int i = 0; i < scoreCount; i++) {
            char line[64];
            snprintf(line, sizeof(line), "%d. %s  %.2fs  %s",
                     i + 1, scores[i].name, scores[i].time, scores[i].date);
            DrawText(line, x, y, 16, DARKGRAY);
            y += 20;
        }
    }

    y += 10;
    char statsLine[64];
    snprintf(statsLine, sizeof(statsLine), "Vitorias: %d  |  Derrotas: %d", stats.wins, stats.losses);
    DrawText(statsLine, x, y, 16, DARKGREEN);
}

// Draws the 3-letter name entry screen after a win
static void drawNameEntry() {
    // Background panel: prompt (20px font) + letters (40px font) + hint (14px font) + padding
    DrawRectangle(WINDOW_WIDTH/2 - 185, WINDOW_HEIGHT/2 - 70, 490, 130, RAYWHITE);

    DrawText("VOCE VENCEU! INSIRA SEU NOME:", WINDOW_WIDTH/2 - 180, WINDOW_HEIGHT/2 - 60, 20, DARKBLUE);

    // Display the 3 letters, highlighting the current one in red
    for (int i = 0; i < 3; i++) {
        char letra[2] = {playerName[i], '\0'};
        Color cor = (i == nameCharIndex) ? RED : DARKGRAY;
        DrawText(letra, WINDOW_WIDTH/2 - 30 + i * 30, WINDOW_HEIGHT/2 - 20, 40, cor);
    }

    DrawText("CIMA/BAIXO: muda letra   DIREITA/ENTER: confirma   ESQUERDA: volta",
             WINDOW_WIDTH/2 - 300, WINDOW_HEIGHT/2 + 40, 14, GRAY);
}

void drawGame(character* player, NpcData* npcData, character** hats, Texture2D playerTexture, Texture2D hatTexture) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw river
    DrawRectangle(0, RIVER_UPPER_BOUNDARY,  WINDOW_WIDTH, RIVER_LOWER_BOUNDARY  - RIVER_UPPER_BOUNDARY,  SKYBLUE);
    // Draw street
    DrawRectangle(0, STREET_UPPER_BOUNDARY, WINDOW_WIDTH, STREET_LOWER_BOUNDARY - STREET_UPPER_BOUNDARY, DARKGRAY);

    // Draw cars
    for (int i = 0; i < *npcData->carCount; i++) {
        Texture2D tex = npcData->carTextures[(*npcData->cars)[i].textureIndex];
        DrawTexturePro(tex,
            (Rectangle){0, 0, tex.width, tex.height},
            (Rectangle){(*npcData->cars)[i].position.x, (*npcData->cars)[i].position.y,
                        (*npcData->cars)[i].width, (*npcData->cars)[i].height},
            (Vector2){0, 0}, 0.0f, WHITE);
    }
    // Draw gators
    for (int i = 0; i < *npcData->gatorCount; i++) {
        Texture2D tex = npcData->gatorTextures[(*npcData->gators)[i].textureIndex];
        DrawTexturePro(tex,
            (Rectangle){0, 0, tex.width, tex.height},
            (Rectangle){(*npcData->gators)[i].position.x, (*npcData->gators)[i].position.y,
                        (*npcData->gators)[i].width, (*npcData->gators)[i].height},
            (Vector2){0, 0}, 0.0f, WHITE);
    }
    // Draw logs
    for (int i = 0; i < *npcData->logCount; i++) {
        Texture2D tex = npcData->logTextures[(*npcData->logs)[i].textureIndex];
        DrawTexturePro(tex,
            (Rectangle){0, 0, tex.width, tex.height},
            (Rectangle){(*npcData->logs)[i].position.x, (*npcData->logs)[i].position.y,
                        (*npcData->logs)[i].width, (*npcData->logs)[i].height},
            (Vector2){0, 0}, 0.0f, WHITE);
    }
    // Draw hats
    for (int i = 0; i < HAT_COUNT; i++) {
        DrawTexturePro(hatTexture,
            (Rectangle){0, 0, hatTexture.width, hatTexture.height},
            (Rectangle){(*hats)[i].position.x, (*hats)[i].position.y,
                        (*hats)[i].width, (*hats)[i].height},
            (Vector2){0, 0}, 0.0f, WHITE);
    }
    // Draw player
    DrawTexturePro(playerTexture,
        (Rectangle){0, 0, playerTexture.width, playerTexture.height},
        (Rectangle){player->position.x, player->position.y, player->width, player->height},
        (Vector2){0, 0}, 0.0f, WHITE);

    // Draw timer during gameplay
    if (!isGameOver && !isEnteringName) {
        char timerText[32];
        snprintf(timerText, sizeof(timerText), "Tempo: %.1fs", elapsedTime);
        DrawRectangle(WINDOW_WIDTH - 165, 5, 155, 28, RAYWHITE);
        DrawText(timerText, WINDOW_WIDTH - 160, 10, 20, DARKGRAY);
    }

    // Draw name entry screen after a win
    if (isEnteringName) {
        drawNameEntry();
    }

    // Draw scoreboard on game over / start screen
    if (isGameOver && !isEnteringName) {
        drawScoreboard();
    }

    // Draw main text overlays
    DrawText(topText,    10,             10,             20, DARKGRAY);
    DrawText(centerText, WINDOW_WIDTH/3, 2*WINDOW_HEIGHT/3, 40, GREEN);

    EndDrawing();
}