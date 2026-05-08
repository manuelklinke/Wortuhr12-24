#ifndef _MATRIXRAIN_H
#define _MATRIXRAIN_H

#include <Adafruit_NeoMatrix.h>
#include "wordclock.h"

void initMatrixRain(stateType_t *state);
void updateMatrixRain(stateType_t *state, Adafruit_NeoMatrix *matrix);

#endif
