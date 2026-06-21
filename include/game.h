#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "character.h"
#include "lane.h"

// Constants
#define DELAY 0.2
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BLOCK_SIZE 30
#define PLAYER_SIZE BLOCK_SIZE
#define PLAYER_STEP BLOCK_SIZE
#define CAR_CHANCE 5
#define GATOR_CHANCE 1
#define LOG_CHANCE 15
#define HAT_COUNT 5

// Global variables
extern int isGameOver;
extern char* screenText;
extern float hopTimer;

// Function prototypes
void initGame(character* player, NpcData* npcData, character** hats);
void updateGame(character* player, NpcData* npcData, character** hats);
void drawGame(character* player, NpcData* npcData, character** hats);
void handlePlayerMovement(character* player);
character* spawnEnemy(character* enemies, int* enemyCount, int enemyChance, char enemyType);
void resetGame(character* player, NpcData* npcData, character** hats);

#endif