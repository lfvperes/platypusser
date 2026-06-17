#include "character.h"

int collision(character a, character b) {
    return (a.position.x < b.position.x + b.width) &&
           (a.position.x + a.width > b.position.x) &&
           (a.position.y < b.position.y + b.height) &&
           (a.position.y + a.height > b.position.y);
}