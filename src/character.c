#include <stdlib.h>
#include "character.h"
#include "game.h"
#include "river.h"

Color colors[MAX_COLORS_COUNT] = {
    DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
    GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
    GREEN, SKYBLUE, PURPLE, BEIGE
};

int collision(character a, character b) {
    return (a.position.x < b.position.x + b.width) &&
           (a.position.x + a.width > b.position.x) &&
           (a.position.y < b.position.y + b.height) &&
           (a.position.y + a.height > b.position.y);
}

character* spawnNpc(NpcData* npcData, int npcChance, char npcType, lane* lanes) {
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
        case 'C':
            spawnColor = colors[GetRandomValue(0, MAX_COLORS_COUNT - 1)];
            npcCount = npcData->carCount;
            npcs = npcData->cars;
            break;
        default:
            return *npcs; // Invalid NPC type, return unchanged
    }

    if (*npcCount < 10 && GetRandomValue(0, 100) < npcChance) {
        int currentLane, speed, length;
        int minLane = 1, maxLane = 4;

        currentLane = GetRandomValue(minLane, maxLane);
        

        // Initialize laneY and speed
        int laneY = lanes[currentLane - 1].yPosition;
        speed = lanes[currentLane - 1].speed;

        // Check if the spawn zone of the lane is occupied
        if (isLaneOccupied(npcData, laneY)) {
            // Try to find another free lane
            int foundFreeLane = 0;
            for (int i = minLane; i <= maxLane; i++) {
                int testLaneY = lanes[i - 1].yPosition;
                speed = lanes[i - 1].speed;

                if (!isLaneOccupied(npcData, testLaneY)) {
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
        (*npcs)[*npcCount].velocity = (Vector2){speed, 0};
        (*npcs)[*npcCount].width = (npcType == 'G') ? GATOR_LENGTH : GetRandomValue(LOG_MIN_LENGTH, LOG_MAX_LENGTH);
        (*npcs)[*npcCount].height = (npcType == 'G') ? GATOR_HEIGHT : LOG_HEIGHT;
        (*npcs)[*npcCount].position = (Vector2){-(*npcs)[*npcCount].width, laneY};
        (*npcs)[*npcCount].color = spawnColor;
        (*npcCount)++;
    }
    return *npcs;
}

void updateNPCPosition(character** npcs, int* npcCount) {
    // Update npc positions and remove out-of-bounds
    for (int i = 0; i < *npcCount; i++) {
        (*npcs)[i].position.x += (*npcs)[i].velocity.x;
        if ((*npcs)[i].position.x > WINDOW_WIDTH) {
            // Remove npc by shifting remaining npcs and reallocating
            for (int j = i; j < *npcCount - 1; j++) {
                (*npcs)[j] = (*npcs)[j + 1];
            }
            (*npcCount)--;
            i--; // Adjust index since we removed an element
            if (*npcCount > 0) {
                *npcs = realloc(*npcs, *npcCount * sizeof(character));
            } else {
                free(*npcs);
                *npcs = NULL;
            }
        }
    }
}
