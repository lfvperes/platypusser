#ifndef LANE_H
#define LANE_H

typedef struct NpcData NpcData;

typedef struct {
    int speed;
    int yPosition;
} lane;

int isLaneOccupied(NpcData* npcData, int laneY);

#endif