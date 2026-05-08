#include <Arduino.h>
#include "gameoflife.h"

namespace {

unsigned long lastGameOfLifeStep = 0;
bool lifeGrid[WIDTH][HEIGHT] = {false};
bool nextLifeGrid[WIDTH][HEIGHT] = {false};
uint8_t lifeAge[WIDTH][HEIGHT] = {0};

void clearEffectGrid(stateType_t *state){
  for(int xPos = 0; xPos < WIDTH; xPos++){
    for(int yPos = 0; yPos < HEIGHT; yPos++){
      state->last_grid[xPos][yPos] = state->grid[xPos][yPos];
      state->grid[xPos][yPos] = 0;
    }
  }
}

uint8_t countLifeNeighbours(int xPos, int yPos){
  uint8_t neighbours = 0;
  for(int dx = -1; dx <= 1; dx++){
    for(int dy = -1; dy <= 1; dy++){
      if(dx == 0 && dy == 0){
        continue;
      }
      int nx = (xPos + dx + WIDTH) % WIDTH;
      int ny = (yPos + dy + HEIGHT) % HEIGHT;
      if(lifeGrid[nx][ny]){
        neighbours++;
      }
    }
  }
  return neighbours;
}

bool hasLivingCells(){
  for(int xPos = 0; xPos < WIDTH; xPos++){
    for(int yPos = 0; yPos < HEIGHT; yPos++){
      if(lifeGrid[xPos][yPos]){
        return true;
      }
    }
  }
  return false;
}

uint16_t gameOfLifeAgeColor(Adafruit_NeoMatrix *matrix, uint8_t age){
  uint8_t phase = age < 30 ? age : 30;

  if(phase <= 10){
    return matrix->Color(255, phase * 25, 0);
  }
  if(phase <= 20){
    uint8_t step = phase - 10;
    return matrix->Color(255 - step * 25, 255, 0);
  }

  uint8_t step = phase - 20;
  return matrix->Color(0, 255 - step * 25, step * 25);
}

}

void seedGameOfLife(stateType_t *state){
  clearEffectGrid(state);
  for(int xPos = 0; xPos < WIDTH; xPos++){
    for(int yPos = 0; yPos < HEIGHT; yPos++){
      lifeGrid[xPos][yPos] = random(100) < 32;
      nextLifeGrid[xPos][yPos] = false;
      lifeAge[xPos][yPos] = lifeGrid[xPos][yPos] ? 1 : 0;
    }
  }
  lastGameOfLifeStep = 0;
}

void updateGameOfLife(stateType_t *state, Adafruit_NeoMatrix *matrix, bool agingColors){
  if(millis() - lastGameOfLifeStep < 500){
    return;
  }
  lastGameOfLifeStep = millis();

  for(int xPos = 0; xPos < WIDTH; xPos++){
    for(int yPos = 0; yPos < HEIGHT; yPos++){
      uint8_t neighbours = countLifeNeighbours(xPos, yPos);
      bool alive = lifeGrid[xPos][yPos];
      nextLifeGrid[xPos][yPos] = alive ? (neighbours == 2 || neighbours == 3) : (neighbours == 3);
    }
  }

  clearEffectGrid(state);
  for(int xPos = 0; xPos < WIDTH; xPos++){
    for(int yPos = 0; yPos < HEIGHT; yPos++){
      lifeGrid[xPos][yPos] = nextLifeGrid[xPos][yPos];
      if(lifeGrid[xPos][yPos]){
        if(lifeAge[xPos][yPos] < 30){
          lifeAge[xPos][yPos]++;
        }
        state->grid[xPos][yPos] = agingColors ? gameOfLifeAgeColor(matrix, lifeAge[xPos][yPos]) : state->Color;
      }else{
        lifeAge[xPos][yPos] = 0;
      }
    }
  }

  if(!hasLivingCells()){
    seedGameOfLife(state);
  }
}
