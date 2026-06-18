#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include "lane.h"

#define MAX_COLORS_COUNT 21

typedef struct {
    Vector2 position;
    Vector2 velocity;
    int width;
    int height;
    Color color;
} character;

typedef struct {
    character** gators;
    int* gatorCount;
    character** logs;
    int* logCount;
    character** cars;
    int* carCount;
} NpcData;

int collision(character a, character b);
character* spawnNpc(NpcData* npcData, int npcChance, char npcType, lane* lanes);
void updateNPCPosition(character** npcs, int* npcCount);

#endif