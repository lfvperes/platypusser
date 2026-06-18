#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"

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
} NpcData;

int collision(character a, character b);
character* spawnNpc(NpcData* npcData, int npcChance, char npcType);

#endif