#include <stdlib.h>
#include "character.h"
#include "game.h"
#include "river.h"
#include "street.h"

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
    int width, height, textureIndex;
    character** npcs;
    int minLane = 1, maxLane;

    // Determine NPC type and set appropriate variables
    switch (npcType) {
        case 'G':
            spawnColor = DARKGREEN;
            npcCount = npcData->gatorCount;  // No dereferencing needed
            npcs = npcData->gators;
            width = GATOR_LENGTH;
            height = GATOR_HEIGHT;
            maxLane = RIVER_LANE_COUNT;
            break;
        case 'L':
            spawnColor = BROWN;
            npcCount = npcData->logCount;    // No dereferencing needed
            npcs = npcData->logs;
            width = GetRandomValue(LOG_MIN_LENGTH, LOG_MAX_LENGTH);
            height = LOG_HEIGHT;
            maxLane = RIVER_LANE_COUNT;
            break;
        case 'C':
            spawnColor = colors[GetRandomValue(0, MAX_COLORS_COUNT - 1)];
            npcCount = npcData->carCount;
            npcs = npcData->cars;
            width = CAR_LENGTH;
            height = CAR_HEIGHT;
            maxLane = STREET_LANE_COUNT;
            textureIndex = GetRandomValue(0, MAX_CAR_TEXTURES - 1);
            break;
        default:
            return *npcs; // Invalid NPC type, return unchanged
    }

    if (*npcCount < 10 && GetRandomValue(0, 100) < npcChance) {
        int currentLane, speed, length;

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
        (*npcs)[*npcCount].width = width;
        (*npcs)[*npcCount].height = height;
        (*npcs)[*npcCount].position = (Vector2){-(*npcs)[*npcCount].width, laneY};
        (*npcs)[*npcCount].color = spawnColor;
        if (npcType == 'C') (*npcs)[*npcCount].textureIndex = textureIndex;
        (*npcCount)++;
    }
    return *npcs;
}

void updateNpcPosition(character** npcs, int* npcCount) {
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

Texture2D* loadTextures(char* basePath, int count) {
    char* fullPath;
    Texture2D* textures = malloc(count * sizeof(Texture2D));
    for (int i = 1; i <= count; i++) {
        fullPath = TextFormat("%s%d.png", basePath, i);
        textures[i-1] = LoadTexture(fullPath);
    }
    return textures;
}