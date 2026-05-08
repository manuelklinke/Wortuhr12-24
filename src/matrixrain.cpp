#include <Arduino.h>
#include "matrixrain.h"

namespace {

unsigned long lastMatrixRainStep = 0;
unsigned long nextMatrixRainStart = 0;
bool matrixRainActive[WIDTH] = {false};
int8_t matrixRainHeadY[WIDTH] = {0};
uint8_t matrixRainLength[WIDTH] = {0};
uint8_t matrixRainHeadBrightness[WIDTH] = {0};
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

uint8_t matrixRainTailBrightness(uint8_t headBrightness, uint8_t segmentIndex, uint8_t chainLength){
  if(segmentIndex == 0){
    return headBrightness;
  }
  if(segmentIndex == 1){
    return headBrightness / 3;
  }
  if(segmentIndex >= chainLength){
    return 0;
  }

  uint8_t remainingSegments = chainLength > 2 ? chainLength - 2 : 1;
  uint16_t fadeStart = headBrightness / 4;
  uint16_t fade = fadeStart * (remainingSegments - (segmentIndex - 2)) / remainingSegments;
  return fade > 8 ? fade : 0;
}

void startMatrixRainChain(){
  for(uint8_t tries = 0; tries < WIDTH; tries++){
    if(matrixRainColumnIndex >= WIDTH){
      shuffleMatrixRainColumns();
    }

    uint8_t column = matrixRainColumnOrder[matrixRainColumnIndex++];
    if(!matrixRainActive[column]){
      matrixRainActive[column] = true;
      matrixRainHeadY[column] = -random(0, 4);
      matrixRainLength[column] = random(4, HEIGHT + 5);
      matrixRainHeadBrightness[column] = random(170, 256);
      return;
    }
  }
}

}

void initMatrixRain(stateType_t *state){
  clearEffectGrid(state);
  for(uint8_t column = 0; column < WIDTH; column++){
    matrixRainActive[column] = false;
    matrixRainHeadY[column] = 0;
    matrixRainLength[column] = 0;
    matrixRainHeadBrightness[column] = 0;
  }
  shuffleMatrixRainColumns();
  lastMatrixRainStep = 0;
  nextMatrixRainStart = 0;
}

void updateMatrixRain(stateType_t *state, Adafruit_NeoMatrix *matrix){
  unsigned long nowMs = millis();
  if(nextMatrixRainStart == 0 || nowMs >= nextMatrixRainStart){
    startMatrixRainChain();
    nextMatrixRainStart = nowMs + random(35, 150);
  }

  if(nowMs - lastMatrixRainStep < 90){
    return;
  }
  lastMatrixRainStep = nowMs;

  clearEffectGrid(state);
  uint16_t rainColor = state->Color == 0 ? matrix->Color(0, 255, 0) : state->Color;
  for(uint8_t column = 0; column < WIDTH; column++){
    if(!matrixRainActive[column]){
      continue;
    }

    matrixRainHeadY[column]++;
    for(uint8_t segment = 0; segment < matrixRainLength[column]; segment++){
      int8_t yPos = matrixRainHeadY[column] - segment;
      if(yPos < 0 || yPos >= HEIGHT){
        continue;
      }

      uint8_t brightness = matrixRainTailBrightness(matrixRainHeadBrightness[column], segment, matrixRainLength[column]);
      if(brightness > 0){
        state->grid[column][yPos] = scaleColor(matrix, rainColor, brightness);
      }
    }

    if(matrixRainHeadY[column] - matrixRainLength[column] >= HEIGHT){
      matrixRainActive[column] = false;
    }
  }
}
