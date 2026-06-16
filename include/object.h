#ifndef OBJECT_H
#define OBJECT_H

#include "raylib.h"

typedef struct {
    Vector2 position;
    Vector2 velocity;
    int width;
    int height;
    Color color;
} object;

int collision(object a, object b);

#endif