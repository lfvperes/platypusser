#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "object.h"

// Constants
#define DELAY 0.2
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PLAYER_SIZE 30
#define PLAYER_STEP PLAYER_SIZE
#define CAR_HEIGHT PLAYER_SIZE
#define CAR_LENGTH PLAYER_SIZE*2
#define CAR_SPEED 3
#define MAX_COLORS_COUNT 21
#define LANE_SIZE 30
#define LANE_1 LANE_SIZE * 1
#define LANE_2 LANE_SIZE * 3
#define LANE_3 LANE_SIZE * 5
#define LANE_4 LANE_SIZE * 7
#define LANE_5 LANE_SIZE * 10
#define LANE_6 LANE_SIZE * 12
#define LANE_7 LANE_SIZE * 14
#define LANE_8 LANE_SIZE * 16
#define STREET_UPPER_BOUNDARY WINDOW_HEIGHT/2 - LANE_SIZE
#define STREET_LOWER_BOUNDARY WINDOW_HEIGHT
#define RIVER_UPPER_BOUNDARY LANE_SIZE
#define RIVER_LOWER_BOUNDARY WINDOW_HEIGHT/2 - 2 * LANE_SIZE
#define CAR_CHANCE 5 // 5% chance to spawn a car each frame

// Global variables
extern Color colors[MAX_COLORS_COUNT];
extern int isGameOver;
extern char* screenText;
extern float hopTimer;
extern int carCount;
extern int gatorCount;

// Function prototypes
void initGame(object* player);
void updateGame(object* player, object** cars, int* carCount, object** gators, int* gatorCount, object** logs, int* logCount);
void drawGame(object player, object* cars, int carCount, object* gators, int gatorCount, object* logs, int logCount);
void handlePlayerMovement(object* player);
object* spawnEnemy(object* enemies, int* enemyCount, int enemyChance, char enemyType);
void resetGame(object* player, object** cars, int* carCount, object** gators, int* gatorCount, object** logs, int* logCount);

#endif