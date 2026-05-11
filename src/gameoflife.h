#ifndef _GAMEOFLIFE_H
#define _GAMEOFLIFE_H

#include <Adafruit_NeoMatrix.h>
#include "wordclock.h"

void seedGameOfLife(stateType_t *state);
void updateGameOfLife(stateType_t *state, Adafruit_NeoMatrix *matrix, bool agingColors);
void setGameOfLifeRandomSeed(bool enabled);
bool isGameOfLifeRandomSeed();
void clearGameOfLifeSeedPattern();
void setGameOfLifeSeedCell(uint8_t xPos, uint8_t yPos, bool alive);
bool getGameOfLifeSeedCell(uint8_t xPos, uint8_t yPos);

#endif
