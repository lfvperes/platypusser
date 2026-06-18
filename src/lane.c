#include "character.h"
#include "river.h"

int isLaneOccupied(NpcData* npcData, int laneY) {
    for (int i = 0; i < *npcData->gatorCount; i++) {
        if ((*npcData->gators)[i].position.y == laneY && (*npcData->gators)[i].position.x < GATOR_LENGTH*2) {
            return 1;
        }
    }
    for (int i = 0; i < *npcData->logCount; i++) {
        if ((*npcData->logs)[i].position.y == laneY && (*npcData->logs)[i].position.x < (*npcData->logs)[i].width*2) {
            return 1;
        }
    }
    for (int i = 0; i < *npcData->carCount; i++) {
        if ((*npcData->cars)[i].position.y == laneY && (*npcData->cars)[i].position.x < (*npcData->cars)[i].width*2) {
            return 1;
        }
    }

    return 0;
}