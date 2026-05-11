#include <Arduino.h>
#include "matrixrain.h"

namespace {

const uint8_t MAX_MATRIX_RAIN_CHAINS = WIDTH * 2;
const uint8_t MIN_ADJACENT_SPEED_DISTANCE = 45;

typedef struct
{
  bool active;
  uint8_t column;
  int8_t headY;
  uint8_t length;
  uint8_t headBrightness;
  uint8_t speed;
  unsigned long lastStep;
} matrixRainChain_t;

unsigned long nextMatrixRainStart = 0;
matrixRainChain_t matrixRainChains[MAX_MATRIX_RAIN_CHAINS] = {};
uint8_t matrixRainColumnOrder[WIDTH] = {0};
uint8_t matrixRainColumnIndex = 0;

void clearEffectGrid(stateType_t *state){
  for(int xPos = 0; xPos < WIDTH; xPos++){
    for(int yPos = 0; yPos < HEIGHT; yPos++){
      state->last_grid[xPos][yPos] = state->grid[xPos][yPos];
      state->grid[xPos][yPos] = 0;
    }
  }
}

uint16_t scaleColor(Adafruit_NeoMatrix *matrix, uint16_t color, uint8_t brightness){
  uint8_t r = ((color >> 11) & 0x1f) * 255 / 31;
  uint8_t g = ((color >> 5) & 0x3f) * 255 / 63;
  uint8_t b = (color & 0x1f) * 255 / 31;
  r = (uint16_t)r * brightness / 255;
  g = (uint16_t)g * brightness / 255;
  b = (uint16_t)b * brightness / 255;
  return matrix->Color(r, g, b);
}

void shuffleMatrixRainColumns(){
  for(uint8_t column = 0; column < WIDTH; column++){
    matrixRainColumnOrder[column] = column;
  }
  for(uint8_t column = WIDTH - 1; column > 0; column--){
    uint8_t otherColumn = random(column + 1);
    uint8_t temp = matrixRainColumnOrder[column];
    matrixRainColumnOrder[column] = matrixRainColumnOrder[otherColumn];
    matrixRainColumnOrder[otherColumn] = temp;
  }
  matrixRainColumnIndex = 0;
}

bool speedFitsNeighbourColumns(uint8_t column, uint8_t speed){
  for(uint8_t chain = 0; chain < MAX_MATRIX_RAIN_CHAINS; chain++){
    if(!matrixRainChains[chain].active){
      continue;
    }

    uint8_t otherColumn = matrixRainChains[chain].column;
    bool directNeighbour = (column > 0 && otherColumn == column - 1) ||
                           (column < WIDTH - 1 && otherColumn == column + 1);
    if(directNeighbour && abs((int)speed - (int)matrixRainChains[chain].speed) < MIN_ADJACENT_SPEED_DISTANCE){
      return false;
    }
  }
  return true;
}

uint8_t chooseMatrixRainSpeed(uint8_t column){
  uint8_t speed = random(65, 240);
  for(uint8_t tries = 0; tries < 10; tries++){
    speed = random(65, 240);
    if(speedFitsNeighbourColumns(column, speed)){
      return speed;
    }
  }

  return speed < 150 ? 225 : 75;
}

bool columnHasFreshChain(uint8_t column){
  for(uint8_t chain = 0; chain < MAX_MATRIX_RAIN_CHAINS; chain++){
    if(matrixRainChains[chain].active &&
       matrixRainChains[chain].column == column &&
       matrixRainChains[chain].headY < 3){
      return true;
    }
  }
  return false;
}

int8_t findFreeMatrixRainChain(){
  for(uint8_t chain = 0; chain < MAX_MATRIX_RAIN_CHAINS; chain++){
    if(!matrixRainChains[chain].active){
      return chain;
    }
  }
  return -1;
}

uint8_t matrixRainTailBrightness(uint8_t headBrightness, uint8_t segmentIndex, uint8_t chainLength){
  if(segmentIndex == 0){
    return headBrightness;
  }
  if(segmentIndex == 1){
    return (uint16_t)headBrightness * 3 / 5;
  }
  if(segmentIndex >= chainLength){
    return 0;
  }

  uint8_t remainingSegments = chainLength > 2 ? chainLength - 2 : 1;
  uint16_t fadeStart = (uint16_t)headBrightness * 2 / 5;
  uint16_t fade = fadeStart * (remainingSegments - (segmentIndex - 2)) / remainingSegments;
  return fade > 14 ? fade : 0;
}

void startMatrixRainChain(unsigned long nowMs){
  int8_t freeChain = findFreeMatrixRainChain();
  if(freeChain < 0){
    return;
  }

  for(uint8_t tries = 0; tries < WIDTH; tries++){
    if(matrixRainColumnIndex >= WIDTH){
      shuffleMatrixRainColumns();
    }

    uint8_t column = matrixRainColumnOrder[matrixRainColumnIndex++];
    if(columnHasFreshChain(column)){
      continue;
    }

    matrixRainChains[freeChain].active = true;
    matrixRainChains[freeChain].column = column;
    matrixRainChains[freeChain].headY = -random(0, 4);
    matrixRainChains[freeChain].length = random(5, HEIGHT + 7);
    matrixRainChains[freeChain].headBrightness = random(215, 256);
    matrixRainChains[freeChain].speed = chooseMatrixRainSpeed(column);
    matrixRainChains[freeChain].lastStep = nowMs - random(0, matrixRainChains[freeChain].speed);
    return;
  }
}

}

void initMatrixRain(stateType_t *state){
  clearEffectGrid(state);
  for(uint8_t chain = 0; chain < MAX_MATRIX_RAIN_CHAINS; chain++){
    matrixRainChains[chain].active = false;
    matrixRainChains[chain].column = 0;
    matrixRainChains[chain].headY = 0;
    matrixRainChains[chain].length = 0;
    matrixRainChains[chain].headBrightness = 0;
    matrixRainChains[chain].speed = 0;
    matrixRainChains[chain].lastStep = 0;
  }
  shuffleMatrixRainColumns();
  nextMatrixRainStart = 0;
}

void updateMatrixRain(stateType_t *state, Adafruit_NeoMatrix *matrix){
  unsigned long nowMs = millis();
  if(nextMatrixRainStart == 0 || nowMs >= nextMatrixRainStart){
    uint8_t newChains = random(1, 4);
    for(uint8_t chain = 0; chain < newChains; chain++){
      startMatrixRainChain(nowMs);
    }
    nextMatrixRainStart = nowMs + random(20, 80);
  }

  clearEffectGrid(state);
  uint8_t brightnessGrid[WIDTH][HEIGHT] = {};
  uint16_t rainColor = state->Color == 0 ? matrix->Color(0, 255, 0) : state->Color;
  for(uint8_t chain = 0; chain < MAX_MATRIX_RAIN_CHAINS; chain++){
    if(!matrixRainChains[chain].active){
      continue;
    }

    if(nowMs - matrixRainChains[chain].lastStep >= matrixRainChains[chain].speed){
      matrixRainChains[chain].lastStep = nowMs;
      matrixRainChains[chain].headY++;
    }

    for(uint8_t segment = 0; segment < matrixRainChains[chain].length; segment++){
      int8_t yPos = matrixRainChains[chain].headY - segment;
      if(yPos < 0 || yPos >= HEIGHT){
        continue;
      }

      uint8_t brightness = matrixRainTailBrightness(matrixRainChains[chain].headBrightness, segment, matrixRainChains[chain].length);
      uint8_t column = matrixRainChains[chain].column;
      if(brightness > brightnessGrid[column][yPos]){
        brightnessGrid[column][yPos] = brightness;
      }
    }

    if(matrixRainChains[chain].headY - matrixRainChains[chain].length >= HEIGHT){
      matrixRainChains[chain].active = false;
    }
  }

  for(uint8_t column = 0; column < WIDTH; column++){
    for(uint8_t yPos = 0; yPos < HEIGHT; yPos++){
      if(brightnessGrid[column][yPos] > 0){
        state->grid[column][yPos] = scaleColor(matrix, rainColor, brightnessGrid[column][yPos]);
      }
    }
  }
}
