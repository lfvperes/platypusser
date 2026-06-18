#include <stdlib.h>
#include "character.h"
#include "game.h"
#include "river.h"

int collision(character a, character b) {
    return (a.position.x < b.position.x + b.width) &&
           (a.position.x + a.width > b.position.x) &&
           (a.position.y < b.position.y + b.height) &&
           (a.position.y + a.height > b.position.y);
}

character* spawnNpc(NpcData* npcData, int npcChance, char npcType) {
    Color spawnColor;
    int* npcCount;
    character** npcs;

    // Determine NPC type and set appropriate variables
    switch (npcType) {
        case 'G':
            spawnColor = DARKGREEN;
            npcCount = npcData->gatorCount;  // No dereferencing needed
            npcs = npcData->gators;
            break;
        case 'L':
            spawnColor = MAROON;
            npcCount = npcData->logCount;    // No dereferencing needed
            npcs = npcData->logs;
            break;
        default:
            return *npcs; // Invalid NPC type, return unchanged
    }

    if (*npcCount < 10 && GetRandomValue(0, 100) < npcChance) {
        int currentLane, speed, length;
        int minLane = 1, maxLane = 4;

        currentLane = GetRandomValue(minLane, maxLane);
        

        // Initialize laneY and speed
        int laneY = riverLanes[currentLane - 1].yPosition;
        speed = riverLanes[currentLane - 1].speed;

        // Check if the spawn zone of the lane is occupied
        if (isRiverLaneSpawnOccupied(*npcData->gators, *npcData->gatorCount, *npcData->logs, *npcData->logCount, laneY)) {
            // Try to find another free lane
            int foundFreeLane = 0;
            for (int i = minLane; i <= maxLane; i++) {
                int testLaneY = riverLanes[i - 1].yPosition;
                speed = riverLanes[i - 1].speed;

                if (!isRiverLaneSpawnOccupied(*npcData->gators, *npcData->gatorCount, *npcData->logs, *npcData->logCount, testLaneY)) {
                    laneY = testLaneY;
                    foundFreeLane = 1;
                    break;
                }
            }
            if (!foundFreeLane) {
                return *npcs; // Skip spawning if all spawn zones are occupied
            }
        }

        // Spawn the NPC on the free lane
        *npcs = realloc(*npcs, (*npcCount + 1) * sizeof(character));
        (*npcs)[*npcCount].position = (Vector2){0, laneY};
        (*npcs)[*npcCount].velocity = (Vector2){speed, 0};
        (*npcs)[*npcCount].width = (npcType == 'G') ? GATOR_LENGTH : GetRandomValue(LOG_MIN_LENGTH, LOG_MAX_LENGTH);
        (*npcs)[*npcCount].height = (npcType == 'G') ? GATOR_HEIGHT : LOG_HEIGHT;
        (*npcs)[*npcCount].color = spawnColor;
        (*npcCount)++;
    }
    return *npcs;
}