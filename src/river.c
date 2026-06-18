#include <stdlib.h>
#include "character.h"
#include "river.h"
#include "game.h"

int isRiverLaneSpawnOccupied(character* gators, int gatorCount, character* logs, int logCount, int laneY) {
    for (int i = 0; i < gatorCount; i++) {
        if (gators[i].position.y == laneY && gators[i].position.x < GATOR_LENGTH * 2) {
            return 1;
        }
    }
    for (int i = 0; i < logCount; i++) {
        if (logs[i].position.y == laneY && logs[i].position.x < logs[i].width * 2) {
            return 1;
        }
    }
    return 0;
}
