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
    int textureIndex;
} character;

typedef struct NpcData {
    character** gators;
    int* gatorCount;
    character** logs;
    int* logCount;
    character** cars;
    int* carCount;
    Texture2D* carTextures;
    Texture2D* gatorTextures;
} NpcData;

int collision(character a, character b);
character* spawnNpc(NpcData* npcData, int npcChance, char npcType, lane* lanes);
void updateNpcPosition(character** npcs, int* npcCount);
Texture2D* loadTextures(const char* basePath, int count);

#endif