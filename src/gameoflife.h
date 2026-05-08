#ifndef _GAMEOFLIFE_H
#define _GAMEOFLIFE_H

#include <Adafruit_NeoMatrix.h>
#include "wordclock.h"

void seedGameOfLife(stateType_t *state);
void updateGameOfLife(stateType_t *state, Adafruit_NeoMatrix *matrix, bool agingColors);

#endif
