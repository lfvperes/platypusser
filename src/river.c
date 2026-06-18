#include <stdlib.h>
#include "character.h"
#include "river.h"
#include "game.h"

int isRiverLaneSpawnOccupied(character* gators, int gatorCount, character* logs, int logCount, int laneY) {
    for (int i = 0; i < gatorCount; i++) {
        if (gators[i].position.y == laneY && gators[i].position.x < GATOR_LENGTH * 2) {
            return 1;
        }
    }
    for (int i = 0; i < logCount; i++) {
        if (logs[i].position.y == laneY && logs[i].position.x < logs[i].width * 2) {
            return 1;
        }
    }
    return 0;
}

character* spawnGator(character* gators, int *gatorCount, character* logs, int *logCount, int gatorChance) {
    if (*gatorCount < 10 && GetRandomValue(0, 100) < gatorChance) {
        int currentLane, speed = RIVER_LANE_1_SPEED, length;
        Color spawnColor;
        int minLane, maxLane;
        
        // Determine lane range and color based on npc type
        minLane = 1;
        maxLane = 4;
        currentLane = GetRandomValue(minLane, maxLane);
        spawnColor = DARKGREEN;
        length = GATOR_LENGTH;

        // Map lane number to Y position
        int laneY;
        switch (currentLane) {
            case 1:
                laneY = RIVER_LANE_1;
                speed = riverLanes[0].speed;
                break;
                case 2:
                laneY = RIVER_LANE_2;
                speed = riverLanes[1].speed;
                break;
            case 3:
                laneY = RIVER_LANE_3;
                speed = riverLanes[2].speed;
                break;
            case 4:
                laneY = RIVER_LANE_4;
                speed = riverLanes[3].speed;
                break;
            default:
                laneY = RIVER_LANE_1;
                speed = riverLanes[0].speed;
                break; // Fallback
        }
            
            // Check if the spawn zone of the lane is occupied
            if (isRiverLaneSpawnOccupied(gators, *gatorCount, logs, *logCount, laneY)) {
                // Try to find another free lane (only in appropriate range for this npc type)
                int foundFreeLane = 0;
                for (int i = minLane; i <= maxLane; i++) {
                    int testLaneY;
                    switch (i) {
                        case 1:
                            testLaneY = RIVER_LANE_1;
                            speed = riverLanes[0].speed;
                            break;
                        case 2:
                            testLaneY = RIVER_LANE_2;
                            speed = riverLanes[1].speed;
                            break;
                        case 3:
                            testLaneY = RIVER_LANE_3;
                            speed = riverLanes[2].speed;
                            break;
                        case 4:
                            testLaneY = RIVER_LANE_4;
                            speed = riverLanes[3].speed;
                            break;
                        default:
                            testLaneY = RIVER_LANE_1;
                            speed = riverLanes[0].speed;
                            break; // Fallback
                    }
                
                if (!isRiverLaneSpawnOccupied(gators, *gatorCount, logs, *logCount, testLaneY)) {
                    laneY = testLaneY;
                    foundFreeLane = 1;
                    break;
                }
            }
            if (!foundFreeLane) {
                return gators; // Skip spawning if all spawn zones are occupied
            }
        }

        // Spawn the gator on the free lane
        gators = realloc(gators, (*gatorCount + 1) * sizeof(character));
        gators[*gatorCount].position = (Vector2){0, laneY};
        gators[*gatorCount].velocity = (Vector2){speed, 0};
        gators[*gatorCount].width = length;
        gators[*gatorCount].height = LOG_HEIGHT;
        gators[*gatorCount].color = spawnColor;
        (*gatorCount)++;
    }
    return gators;
}


character* spawnLog(character* gators, int *gatorCount, character* logs, int *logCount, int logChance) {
    if (*logCount < 10 && GetRandomValue(0, 100) < logChance) {
        int currentLane, speed = RIVER_LANE_1_SPEED, length;
        Color spawnColor;
        int minLane, maxLane;
        
        // Determine lane range and color based on npc type
        minLane = 1;
        maxLane = 4;
        currentLane = GetRandomValue(minLane, maxLane);
        spawnColor = MAROON;
        length = GetRandomValue(LOG_MIN_LENGTH, LOG_MAX_LENGTH);
        
        // Map lane number to Y position
        int laneY;
        switch (currentLane) {
            case 1:
                laneY = RIVER_LANE_1;
                speed = riverLanes[0].speed;
                break;
            case 2:
                laneY = RIVER_LANE_2;
                speed = riverLanes[1].speed;
                break;
            case 3:
                laneY = RIVER_LANE_3;
                speed = riverLanes[2].speed;
                break;
            case 4:
                laneY = RIVER_LANE_4;
                speed = riverLanes[3].speed;
                break;
            default:
                laneY = RIVER_LANE_1;
                speed = riverLanes[0].speed;
                break; // Fallback
        }

        // Check if the spawn zone of the lane is occupied
        if (isRiverLaneSpawnOccupied(gators, *gatorCount, logs, *logCount, laneY)) {
            // Try to find another free lane (only in appropriate range for this npc type)
            int foundFreeLane = 0;
            for (int i = minLane; i <= maxLane; i++) {
                int testLaneY;
                switch (i) {
                    case 1:
                        testLaneY = RIVER_LANE_1;
                        speed = riverLanes[0].speed;
                        break;
                    case 2:
                        testLaneY = RIVER_LANE_2;
                        speed = riverLanes[1].speed;
                        break;
                    case 3:
                        testLaneY = RIVER_LANE_3;
                        speed = riverLanes[2].speed;
                        break;
                    case 4:
                        testLaneY = RIVER_LANE_4;
                        speed = riverLanes[3].speed;
                        break;
                    default:
                        testLaneY = RIVER_LANE_1;
                        speed = riverLanes[0].speed;
                        break; // Fallback
                }
                
                if (!isRiverLaneSpawnOccupied(gators, *gatorCount, logs, *logCount, testLaneY)) {
                    laneY = testLaneY;
                    foundFreeLane = 1;
                    break;
                }
            }
            if (!foundFreeLane) {
                return logs; // Skip spawning if all spawn zones are occupied
            }
        }

        // Spawn the log on the free lane
        logs = realloc(logs, (*logCount + 1) * sizeof(character));
        logs[*logCount].position = (Vector2){0, laneY};
        logs[*logCount].velocity = (Vector2){speed, 0};
        logs[*logCount].width = length;
        logs[*logCount].height = LOG_HEIGHT;
        logs[*logCount].color = spawnColor;
        (*logCount)++;
    }
    return logs;
}

void updateGatorPosition(character** gators, int* gatorCount) {
    // Update gator positions and remove out-of-bounds
    for (int i = 0; i < *gatorCount; i++) {
        (*gators)[i].position.x += (*gators)[i].velocity.x;
        if ((*gators)[i].position.x > WINDOW_WIDTH) {
            // Remove gator by shifting remaining gators and reallocating
            for (int j = i; j < *gatorCount - 1; j++) {
                (*gators)[j] = (*gators)[j + 1];
            }
            (*gatorCount)--;
            i--; // Adjust index since we removed an element
            if (*gatorCount > 0) {
                *gators = realloc(*gators, *gatorCount * sizeof(character));
            } else {
                free(*gators);
                *gators = NULL;
            }
        }
    }
}

void updateLogPosition(character** logs, int* logCount) {
    // Update log positions and remove out-of-bounds
    for (int i = 0; i < *logCount; i++) {
        (*logs)[i].position.x += (*logs)[i].velocity.x;
        if ((*logs)[i].position.x > WINDOW_WIDTH) {
            // Remove log by shifting remaining logs and reallocating
            for (int j = i; j < *logCount - 1; j++) {
                (*logs)[j] = (*logs)[j + 1];
            }
            (*logCount)--;
            i--; // Adjust index since we removed an element
            if (*logCount > 0) {
                *logs = realloc(*logs, *logCount * sizeof(character));
            } else {
                free(*logs);
                *logs = NULL;
            }
        }
    }
}