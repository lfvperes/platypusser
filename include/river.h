#ifndef RIVER_H
#define RIVER_H

#include "game.h"

// common
#define RIVER_MAX_SPEED 2
// gators
#define GATOR_HEIGHT BLOCK_SIZE
#define GATOR_LENGTH BLOCK_SIZE*2
// logs
#define LOG_HEIGHT BLOCK_SIZE
#define LOG_MIN_LENGTH BLOCK_SIZE*1
#define LOG_MAX_LENGTH BLOCK_SIZE*3
// river lanes
#define RIVER_LANE_SIZE BLOCK_SIZE
#define RIVER_LANE_1 RIVER_LANE_SIZE * 1
#define RIVER_LANE_2 RIVER_LANE_SIZE * 3
#define RIVER_LANE_3 RIVER_LANE_SIZE * 5
#define RIVER_LANE_4 RIVER_LANE_SIZE * 7
// river boundaries
#define RIVER_UPPER_BOUNDARY WINDOW_HEIGHT/2 - RIVER_LANE_SIZE
#define RIVER_LOWER_BOUNDARY WINDOW_HEIGHT

character* spawnGator(character* gators, int *gatorCount, character* logs, int *logCount, int logChance);
character* spawnLog(character* gators, int *gatorCount, character* logs, int *logCount, int logChance);
void updateGatorPosition(character** gators, int* gatorCount);
void updateLogPosition(character** logs, int* logCount);
int isRiverLaneSpawnOccupied(character* gators, int gatorCount, character* logs, int logCount, int laneY);

#endif