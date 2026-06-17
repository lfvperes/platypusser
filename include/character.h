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

int collision(character a, character b);

#endif