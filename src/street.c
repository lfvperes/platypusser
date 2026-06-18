#include <stdlib.h>
#include "character.h"
#include "street.h"

Color colors[MAX_COLORS_COUNT] = {
    DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
    GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
    GREEN, SKYBLUE, PURPLE, BEIGE
};

int isStreetLaneSpawnOccupied(character* cars, int carCount, int laneY) {
    for (int i = 0; i < carCount; i++) {
        if (cars[i].position.y == laneY && cars[i].position.x < CAR_LENGTH * 2) {
            return 1;
        }
    }
    return 0;
}

character* spawnCar(character* cars, int* carCount, int carChance) {
    if (*carCount < 10 && GetRandomValue(0, 100) < carChance) {
        int lane;
        Color spawnColor;
        int minLane, maxLane;
        
        minLane = 1;
        maxLane = 4;
        lane = GetRandomValue(minLane, maxLane);
        spawnColor = colors[GetRandomValue(0, MAX_COLORS_COUNT - 1)];

        // Map lane number to Y position
        int laneY;
        switch (lane) {
            case 1: laneY = LANE_1; break;
            case 2: laneY = LANE_2; break;
            case 3: laneY = LANE_3; break;
            case 4: laneY = LANE_4; break;
            default: laneY = LANE_1; break; // Fallback
        }

        // Check if the spawn zone of the lane is occupied
        if (isStreetLaneSpawnOccupied(cars, *carCount, laneY)) {
            // Try to find another free lane (only in appropriate range for this car type)
            int foundFreeLane = 0;
            for (int i = minLane; i <= maxLane; i++) {
                int testLaneY;
                switch (i) {
                    case 1: testLaneY = LANE_1; break;
                    case 2: testLaneY = LANE_2; break;
                    case 3: testLaneY = LANE_3; break;
                    case 4: testLaneY = LANE_4; break;
                    default: testLaneY = LANE_1; break; // Fallback
                }
                
                if (!isStreetLaneSpawnOccupied(cars, *carCount, testLaneY)) {
                    laneY = testLaneY;
                    foundFreeLane = 1;
                    break;
                }
            }
            if (!foundFreeLane) {
                return cars; // Skip spawning if all spawn zones are occupied
            }
        }

        // Spawn the car on the free lane
        cars = realloc(cars, (*carCount + 1) * sizeof(character));
        cars[*carCount].position = (Vector2){0, laneY};
        cars[*carCount].velocity = (Vector2){CAR_SPEED, 0};
        cars[*carCount].width = CAR_LENGTH;
        cars[*carCount].height = CAR_HEIGHT;
        cars[*carCount].color = spawnColor;
        (*carCount)++;
    }
    return cars;
}