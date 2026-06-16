#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "object.h"

// Constants
#define DELAY 0.2
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PLAYER_STEP 50
#define PLAYER_SIZE 50
#define CAR_HEIGHT 50
#define CAR_LENGTH 100
#define CAR_SPEED 3
#define MAX_COLORS_COUNT 21
#define LANE_1 300
#define LANE_2 400
#define LANE_3 500
#define STREET_UPPER_BOUNDARY 300
#define STREET_LOWER_BOUNDARY (WINDOW_HEIGHT - PLAYER_SIZE)
#define CAR_CHANCE 5 // 5% chance to spawn a car each frame

// Global variables
extern Color colors[MAX_COLORS_COUNT];
extern int isGameOver;
extern char* screenText;
extern float hopTimer;
extern int carCount;

// Function prototypes
void initGame(object* player);
void updateGame(object* player, object** cars, int* carCount);
void drawGame(object player, object* cars, int carCount);
void handlePlayerMovement(object* player);
object* spawnCar(object* cars, int* carCount, int carChance);
void resetGame(object* player, object** cars, int* carCount);

#endif