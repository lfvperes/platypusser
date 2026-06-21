#ifndef STREET_H
#define STREET_H

#include "game.h"

// cars
#define CAR_HEIGHT BLOCK_SIZE
#define CAR_LENGTH BLOCK_SIZE*2
#define CAR_SPEED 3
// colors
#define MAX_COLORS_COUNT 21
// street lanes
#define LANE_SIZE BLOCK_SIZE
#define LANE_1 LANE_SIZE * 10
#define LANE_2 LANE_SIZE * 12
#define LANE_3 LANE_SIZE * 14
#define LANE_4 LANE_SIZE * 16
#define STREET_MAX_SPEED 4
#define STREET_MIN_SPEED 1
// street boundaries
#define STREET_UPPER_BOUNDARY WINDOW_HEIGHT/2 
#define STREET_LOWER_BOUNDARY WINDOW_HEIGHT - 5 *LANE_SIZE

#endif