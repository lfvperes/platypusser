#ifndef LANE_H
#define LANE_H

#define STREET_LANE_COUNT 5
#define RIVER_LANE_COUNT 5

typedef struct NpcData NpcData;

typedef struct {
    int speed;
    int yPosition;
} lane;

int isLaneOccupied(NpcData* npcData, int laneY);

#endif