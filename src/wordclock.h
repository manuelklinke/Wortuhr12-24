#include <RTClib.h>

#define WIDTH 24                // width of LED matirx
#define HEIGHT 12 

#define LAYOUT2

#define BRIGHTNESS_DAY 128
#define BRIGHTNESS_NIGHT 64

#define MODE_WORD_CLOCK 0
#define MODE_WORD_CLOCK_SECONDS 1
#define MODE_WORD_CLOCK_COUNT_DOWN 2
#define MODE_WORD_CLOCK_DAYTIME 3
#define MODE_MARQUEE_TIME 4
#define MODE_BINARY_CLOCK 5
#define MODE_RESISTOR_COLOR_CODE_CLOCK 6
#define MODE_GAME_OF_LIFE 7
#define MODE_MATRIX_RAIN 8
#define MODE_TEMPERATURE 9
#define MODE_RATE_TEMPERATURE 10
#define MODE_TIME_FOR_FACTS 11
#define MODE_DISPLAY_SPECIAL_EVENTS 12


#define WORD_CLOCK_MODE_SHORT 0
#define WORD_CLOCK_MODE_LONG 1
#define WORD_CLOCK_MODE_EAST 2
#define WORD_CLOCK_MODE_COUNTDOWN_SECOND_HALF_MINUTES 3

#define COLOR_MODE_MONO 0
#define COLOR_MODE_MIXED 1
#define COLOR_MODE_RAINBOW 2

#define MAX_COLORS 7

typedef struct 
{
  DateTime Now;
  int Mode;
  int Word_Clock_Mode;
  int Color_Mode;
  uint16_t Color;
  uint8_t Color_Counter;
  uint16_t last_Color;
  // representation of matrix as array
  uint16_t grid[WIDTH][HEIGHT] = {0};
  uint16_t last_grid[WIDTH][HEIGHT] = {0};

}stateType_t;