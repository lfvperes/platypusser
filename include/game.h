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
#define CAR_CHANCE 5 // 5% chance to spawn a car each frame

// Global variables
extern int isGameOver;
extern char* screenText;
extern float hopTimer;
extern int carCount;
extern int gatorCount;
extern int logCount;

// Function prototypes
void initGame(character* player, NpcData* npcData);
void updateGame(character* player, character** cars, int* carCount, character** gators, int* gatorCount, character** logs, int* logCount);
void drawGame(character player, character* cars, int carCount, character* gators, int gatorCount, character* logs, int logCount);
void handlePlayerMovement(character* player);
character* spawnEnemy(character* enemies, int* enemyCount, int enemyChance, char enemyType);
void resetGame(character* player, NpcData* npcData);

#endif