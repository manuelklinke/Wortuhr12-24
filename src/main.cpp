#include <Arduino.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>       //https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_NeoMatrix.h> //https://github.com/adafruit/Adafruit_NeoMatrix
#include <Adafruit_NeoPixel.h>

/*
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
0  E  S  B  I  S  T  I  N  V  I  E  L  E  N  Z  E  I  T  F  Ü  N  F  Ü  R
1  D  R  E  I  N  E  R  S  E  C  H  S  I  E  B  E  N  V  I  E  R  T  E  L
2  W  E  N  I  G  E  Z  W  E  I  N  E  U  N  A  C  H  T  S  P  Ä  T  E  R
3  Z  U  N  D  E  L  F  Ü  N  F  Z  I  G  V  I  E  R  Z  I  G  U  T  E  N
4  Z  E  H  N  U  L  L  Z  W  Ö  L  F  R  Ü  H  E  R  D  R  E  I  ß  I  G
5  Z  W  A  N  Z  I  G  A  M  I  N  U  T  E  N  I  X  N  A  C  H  V  O  R
6  H  A  L  B  I  E  R  W  O  C  H  E  N  E  N  D  E  T  A  G  E  N  I  E
7  D  R  E  I  V  I  E  R  T  E  L  F  Ü  N  F  N  E  U  N  A  C  H  T  S
8  S  E  C  H  S  I  E  B  E  N  E  I  N  S  Z  E  H  N  Z  W  E  I  S  T
9  Z  W  Ö  L  F  K  A  F  F  E  E  U  H  R  I  P  R  Ä  A  B  E  N  D  S
10 U  M  O  R  G  E  N  S  V  O  R  M  I  T  T  A  G  S  T  U  N  D  E  N
11 G  R  A  D  U  R  L  A  U  B  Z  U  H  R  W  A  R  M  I  R  K  A  L  T

*/

#define NEOPIXEL_PIN D4
#define WIDTH 24                // width of LED matirx
#define HEIGHT 12           // height of LED matrix 
#define BRIGHTNESS_DAY 128
#define BRIGHTNESS_NIGHT 64

#define MODE_WORD_CLOCK 0
#define MODE_WORD_CLOCK_SECONDS 1
#define MODE_WORD_CLOCK_COUNT_DOWN 2
#define MODE_MARQUEE_TIME 3
#define MODE_BINARY_CLOCK 4
#define MODE_RESISTOR_COLOR_CODE_CLOCK 5
#define MODE_GAME_OF_LIFE 6
#define MODE_MATRIX_RAIN 7


#define WORD_CLOCK_MODE_SHORT 0
#define WORD_CLOCK_MODE_LONG 1
#define WORD_CLOCK_MODE_EAST 2
#define WORD_CLOCK_MODE_COUNTDOWN_SECOND_HALF_MINUTES 3

#define COLOR_MODE_MONO 0
#define COLOR_MODE_MIXED 1
#define COLOR_MODE_RAINBOW 2

#define MAX_COLORS 7

RTC_DS1307 RTC; // Establish clock object

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(WIDTH, HEIGHT, NEOPIXEL_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS    + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);


// define color modes
const uint16_t colors[] = {
  matrix.Color(255, 255, 255),  // white (in this mode together with colorwheel)
  matrix.Color(255, 0, 0),      // red
  matrix.Color(255, 255, 0),    // yellow
  matrix.Color(255, 0, 200),    // magenta
  matrix.Color(128, 128, 128),  // white (darker)
  matrix.Color(0, 255, 0),      // green 
  matrix.Color(0, 0, 255) };    // blue



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

static stateType_t State;

void resetGrid(){
  //set all the array values to 0 (off):
  for(int i = 0; i < WIDTH; i++) {
    for(int j = 0; j < HEIGHT; j++) {
      State.last_grid[i][j] = State.grid[i][j];
      State.grid[i][j] = 0;
    }
  }
}

void draw_ES_IST(uint16_t color){
  // ES
  State.grid[0][0] = color;
  State.grid[1][0] = color;
  //IST
  State.grid[3][0] = color;
  State.grid[4][0] = color;
  State.grid[5][0] = color;
}

void draw_Min_EIN(uint16_t color){
  State.grid[2][1] = color;
  State.grid[3][1] = color;
  State.grid[4][1] = color;
}

void draw_Min_EINE(uint16_t color){
  draw_Min_EIN(color);
  State.grid[5][1] = color;
}

void draw_Min_EINER(uint16_t color){
  draw_Min_EINE(color);
  State.grid[6][1] = color;
}

void draw_Min_ZWEI(uint16_t color){
  State.grid[6][2] = color;
  State.grid[7][2] = color;
  State.grid[8][2] = color;
  State.grid[9][2] = color;
}

void draw_Min_DREI(uint16_t color){
  State.grid[0][1] = color;
  State.grid[1][1] = color;
  State.grid[2][1] = color;
  State.grid[3][1] = color;
}

void draw_Min_VIER(uint16_t color){
  State.grid[17][1] = color;
  State.grid[18][1] = color;
  State.grid[19][1] = color;
  State.grid[20][1] = color;
}

void draw_Min_FUENF(uint16_t color){
  State.grid[18][0] = color;
  State.grid[19][0] = color;
  State.grid[20][0] = color;
  State.grid[21][0] = color;
}

void draw_Min_SECHS(uint16_t color){
  State.grid[7][1] = color;
  State.grid[8][1] = color;
  State.grid[9][1] = color;
  State.grid[10][1] = color;
  State.grid[11][1] = color;
}

void draw_Min_SIEB(uint16_t color){
  State.grid[11][1] = color;
  State.grid[12][1] = color;
  State.grid[13][1] = color;
  State.grid[14][1] = color;
}

void draw_Min_SIEBEN(uint16_t color){
  draw_Min_SIEB(color);
  State.grid[15][1] = color;
  State.grid[16][1] = color;
}

void draw_Min_ACHT(uint16_t color){
  State.grid[14][2] = color;
  State.grid[15][2] = color;
  State.grid[16][2] = color;
  State.grid[17][2] = color;
}

void draw_Min_NEUN(uint16_t color){
  State.grid[10][2] = color;
  State.grid[11][2] = color;
  State.grid[12][2] = color;
  State.grid[13][2] = color;
}

void draw_Min_ZEHN(uint16_t color){
  State.grid[0][4] = color;
  State.grid[1][4] = color;
  State.grid[2][4] = color;
  State.grid[3][4] = color;
}

void draw_Min_ELF(uint16_t color){
  State.grid[4][3] = color;
  State.grid[5][3] = color;
  State.grid[6][3] = color;
}

void draw_Min_ZWOELF(uint16_t color){
  State.grid[7][4] = color;
  State.grid[8][4] = color;
  State.grid[9][4] = color;
  State.grid[10][4] = color;
}

void draw_Min_UND(uint16_t color){
  State.grid[1][3] = color;
  State.grid[2][3] = color;
  State.grid[3][3] = color;
}

void draw_Min_ZWANZIG(uint16_t color){
  State.grid[0][5] = color;
  State.grid[1][5] = color;
  State.grid[2][5] = color;
  State.grid[3][5] = color;
  State.grid[4][5] = color;
  State.grid[5][5] = color;
  State.grid[6][5] = color;
}

void draw_Min_DREISSIG(uint16_t color){
  State.grid[17][4] = color;
  State.grid[18][4] = color;
  State.grid[19][4] = color;
  State.grid[20][4] = color;
  State.grid[21][4] = color;
  State.grid[22][4] = color;
  State.grid[23][4] = color;
}

void draw_Min_VIERZIG(uint16_t color){
  State.grid[13][3] = color;
  State.grid[14][3] = color;
  State.grid[15][3] = color;
  State.grid[16][3] = color;
  State.grid[17][3] = color;
  State.grid[18][3] = color;
  State.grid[19][3] = color;
}

void draw_Min_FUENFZIG(uint16_t color){
  State.grid[6][3] = color;
  State.grid[7][3] = color;
  State.grid[8][3] = color;
  State.grid[9][3] = color;
  State.grid[10][3] = color;
  State.grid[11][3] = color;
  State.grid[12][3] = color;
}

void draw_MINUTE(uint16_t color){
  State.grid[8][5] = color;
  State.grid[9][5] = color;
  State.grid[10][5] = color;
  State.grid[11][5] = color;
  State.grid[12][5] = color;
  State.grid[13][5] = color;
}

void draw_MINUTEN(uint16_t color){
  draw_MINUTE(color);
  State.grid[14][5] = color;
}

void draw_VOR(uint16_t color){
  State.grid[21][5] = color;
  State.grid[22][5] = color;
  State.grid[23][5] = color;
}

void draw_NACH(uint16_t color){
  State.grid[17][5] = color;
  State.grid[18][5] = color;
  State.grid[19][5] = color;
  State.grid[20][5] = color;
}

void draw_HALB(uint16_t color){
  State.grid[0][6] = color;
  State.grid[1][6] = color;
  State.grid[2][6] = color;
  State.grid[3][6] = color;
}

void draw_VIERTEL(uint16_t color){
  draw_Min_VIER(color);
  State.grid[21][1] = color;
  State.grid[22][1] = color;
  State.grid[23][1] = color;
}

void draw_Std_NULL(uint16_t color){
  State.grid[3][4] = color;
  State.grid[4][4] = color;
  State.grid[5][4] = color;
  State.grid[6][4] = color;
}

void draw_Std_EIN(uint16_t color){
  State.grid[10][8] = color;
  State.grid[11][8] = color;
  State.grid[12][8] = color;
}

void draw_Std_EINS(uint16_t color){
  draw_Std_EIN(color);
  State.grid[13][8] = color;
}

void draw_Std_ZWEI(uint16_t color){
  State.grid[18][8] = color;
  State.grid[19][8] = color;
  State.grid[20][8] = color;
  State.grid[21][8] = color;
}

void draw_Std_DREI(uint16_t color){
  State.grid[0][7] = color;
  State.grid[1][7] = color;
  State.grid[2][7] = color;
  State.grid[3][7] = color;
}

void draw_Std_VIER(uint16_t color){
  State.grid[4][7] = color;
  State.grid[5][7] = color;
  State.grid[6][7] = color;
  State.grid[7][7] = color;
}

void draw_Std_FUENF(uint16_t color){
  State.grid[11][7] = color;
  State.grid[12][7] = color;
  State.grid[13][7] = color;
  State.grid[14][7] = color;
}

void draw_Std_SECHS(uint16_t color){
  State.grid[0][8] = color;
  State.grid[1][8] = color;
  State.grid[2][8] = color;
  State.grid[3][8] = color;
  State.grid[4][8] = color;
}

void draw_Std_SIEBEN(uint16_t color){
  State.grid[4][8] = color;
  State.grid[5][8] = color;
  State.grid[6][8] = color;
  State.grid[7][8] = color;
  State.grid[8][8] = color;
  State.grid[9][8] = color;
}

void draw_Std_ACHT(uint16_t color){
  State.grid[19][7] = color;
  State.grid[20][7] = color;
  State.grid[21][7] = color;
  State.grid[22][7] = color;
}

void draw_Std_NEUN(uint16_t color){
  State.grid[15][7] = color;
  State.grid[16][7] = color;
  State.grid[17][7] = color;
  State.grid[18][7] = color;
}

void draw_Std_ZEHN(uint16_t color){
  State.grid[14][8] = color;
  State.grid[15][8] = color;
  State.grid[16][8] = color;
  State.grid[17][8] = color;
}

void draw_Std_ELF(uint16_t color){
  State.grid[9][7] = color;
  State.grid[10][7] = color;
  State.grid[11][7] = color;
}

void draw_Std_ZWOELF(uint16_t color){
  State.grid[0][9] = color;
  State.grid[1][9] = color;
  State.grid[2][9] = color;
  State.grid[3][9] = color;
  State.grid[4][9] = color;
}

void draw_UHR1(uint16_t color){
  State.grid[11][9] = color;
  State.grid[12][9] = color;
  State.grid[13][9] = color;
}

void draw_UHR2(uint16_t color){
  State.grid[11][11] = color;
  State.grid[12][11] = color;
  State.grid[13][11] = color;
}

void draw_MORGEN(uint16_t color){
  State.grid[1][10] = color;
  State.grid[2][10] = color;
  State.grid[3][10] = color;
  State.grid[4][10] = color;
  State.grid[5][10] = color;
  State.grid[6][10] = color;
}

void draw_MORGENS(uint16_t color){
  draw_MORGEN(color);
  State.grid[7][10] = color;
}

void draw_MITTAG(uint16_t color){
  State.grid[11][10] = color;
  State.grid[12][10] = color;
  State.grid[13][10] = color;
  State.grid[14][10] = color;
  State.grid[13][10] = color;
  State.grid[14][10] = color;
}

void draw_MITTAGS(uint16_t color){
  draw_MITTAG(color);
  State.grid[15][10] = color;
}

void draw_VORMITTAG(uint16_t color){
  State.grid[8][10] = color;
  State.grid[9][10] = color;
  State.grid[10][10] = color;
  draw_MITTAG(color);
}

void draw_VORMITTAGS(uint16_t color){
  draw_VORMITTAG(color);
  State.grid[11][10] = color;
}

void draw_ABEND(uint16_t color){
  State.grid[18][9] = color;
  State.grid[19][9] = color;
  State.grid[20][9] = color;
  State.grid[21][9] = color;
  State.grid[22][9] = color;
}

void draw_ABENDS(uint16_t color){
  draw_ABEND(color);
  State.grid[23][9] = color;
}

void draw_PRAEABEND(uint16_t color){
  State.grid[15][9] = color;
  State.grid[16][9] = color;
  State.grid[17][9] = color;
  draw_ABEND(color);
}

void draw_PRAEABENDS(uint16_t color){
  draw_PRAEABEND(color);
  State.grid[23][9] = color;
}

void draw_NACHT(uint16_t color){
  State.grid[18][7] = color;
  draw_Std_ACHT(color);
}

void draw_NACHTS(uint16_t color){
  draw_NACHT(color);
  State.grid[23][7] = color;
}

void draw_ZEIT(uint16_t color){
  State.grid[14][0] = color;
  State.grid[15][0] = color;
  State.grid[16][0] = color;
  State.grid[17][0] = color;
}

void draw_TAG(uint16_t color){
  State.grid[17][6] = color;
  State.grid[18][6] = color;
  State.grid[19][6] = color;
}

void draw_TAGE(uint16_t color){
  draw_TAG(color);
  State.grid[20][6] = color;
}

void draw_TAGEN(uint16_t color){
  draw_TAGE(color);
  State.grid[21][6] = color;
}

void draw_WOCHE(uint16_t color){
  State.grid[7][6] = color;
  State.grid[8][6] = color;
  State.grid[9][6] = color;
  State.grid[10][6] = color;
  State.grid[11][6] = color;
}

void draw_WOCHEN(uint16_t color){
  draw_WOCHE(color);
  State.grid[12][6] = color;
}

void draw_ENDE(uint16_t color){
  State.grid[13][6] = color;
  State.grid[14][6] = color;
  State.grid[15][6] = color;
  State.grid[16][6] = color;
}

void draw_WOCHENENDE(uint16_t color){
  draw_WOCHEN(color);
  draw_ENDE(color);
}

void draw_NIE(uint16_t color){
  State.grid[21][6] = color;
  State.grid[22][6] = color;
  State.grid[23][6] = color;
}

void draw_GENIE(uint16_t color){
  State.grid[19][6] = color;
  State.grid[20][6] = color;
  draw_NIE(color);
}

void draw_NIX(uint16_t color){
  State.grid[14][5] = color;
  State.grid[15][5] = color;
  State.grid[16][5] = color;
}

void draw_VIEL(uint16_t color){
  State.grid[8][0] = color;
  State.grid[9][0] = color;
  State.grid[10][0] = color;
  State.grid[11][0] = color;
}

void draw_VIELE(uint16_t color){
  draw_VIEL(color);
  State.grid[12][0] = color;
}

void draw_WENIG(uint16_t color){
  State.grid[0][2] = color;
  State.grid[1][2] = color;
  State.grid[2][2] = color;
  State.grid[3][2] = color;
  State.grid[4][2] = color;
}

void draw_WENIGE(uint16_t color){
  draw_WENIG(color);
  State.grid[5][2] = color;
}

void draw_SPAET(uint16_t color){
  State.grid[18][2] = color;
  State.grid[19][2] = color;
  State.grid[20][2] = color;
  State.grid[21][2] = color;
}

void draw_SPAETER(uint16_t color){
  draw_SPAET(color);
  State.grid[22][2] = color;
  State.grid[23][2] = color;
}

void draw_FUER(uint16_t color){
  State.grid[21][0] = color;
  State.grid[22][0] = color;
  State.grid[23][0] = color;
}

void draw_KAFFEE(uint16_t color){
  State.grid[5][9] = color;
  State.grid[6][9] = color;
  State.grid[7][9] = color;
  State.grid[8][9] = color;
  State.grid[9][9] = color;
  State.grid[10][9] = color;
}

void draw_EIS(uint16_t color){
  State.grid[20][8] = color;
  State.grid[21][8] = color;
  State.grid[22][8] = color;
}

void draw_BIER(uint16_t color){
  State.grid[3][6] = color;
  State.grid[4][6] = color;
  State.grid[5][6] = color;
  State.grid[6][6] = color;
}

void draw_WEIN(uint16_t color){
  State.grid[7][2] = color;
  State.grid[8][2] = color;
  State.grid[9][2] = color;
  State.grid[10][2] = color;
}

void draw_GUT(uint16_t color){
  State.grid[19][3] = color;
  State.grid[20][3] = color;
  State.grid[21][3] = color;
}

void draw_GUTE(uint16_t color){
  draw_GUT(color);
  State.grid[22][3] = color;
}

void draw_GUTEN(uint16_t color){
  draw_GUTE(color);
  State.grid[23][3] = color;
}

void draw_NEIN(uint16_t color){
  State.grid[9][8] = color;
  State.grid[10][8] = color;
  State.grid[11][8] = color;
  State.grid[12][8] = color;
}

void draw_GRAD(uint16_t color){
  State.grid[0][11] = color;
  State.grid[1][11] = color;
  State.grid[2][11] = color;
  State.grid[3][11] = color;
}

void draw_URLAUB(uint16_t color){
  State.grid[4][11] = color;
  State.grid[5][11] = color;
  State.grid[6][11] = color;
  State.grid[7][11] = color;
  State.grid[8][11] = color;
  State.grid[9][11] = color;
}

void draw_ZU(uint16_t color){
  State.grid[10][11] = color;
  State.grid[11][11] = color;
}

void draw_WARM(uint16_t color){
  State.grid[14][11] = color;
  State.grid[15][11] = color;
  State.grid[16][11] = color;
  State.grid[17][11] = color;
}

void draw_KALT(uint16_t color){
  State.grid[20][11] = color;
  State.grid[21][11] = color;
  State.grid[22][11] = color;
  State.grid[23][11] = color;
}

void draw_MI1(uint16_t color){
  State.grid[8][5] = color;
  State.grid[9][5] = color;
}

void draw_MI2(uint16_t color){
  State.grid[11][10] = color;
  State.grid[12][10] = color;
}

void draw_MI3(uint16_t color){
  State.grid[17][11] = color;
  State.grid[18][11] = color;
}

void draw_MIMIMI(uint16_t color){
  draw_MI1(color);
  draw_MI2(color);
  draw_MI3(color);
}

void draw_IN(uint16_t color){
  State.grid[6][0] = color;
  State.grid[7][0] = color;
}

void draw_IST2(uint16_t color){
  State.grid[21][8] = color;
  State.grid[22][8] = color;
  State.grid[23][8] = color;
}

void draw_AM(uint16_t color){
  State.grid[7][5] = color;
  State.grid[8][5] = color;
}

void drawWordClockMin(){
  uint8_t Min = State.Now.minute();
  if(Min != 0){
    if(Min == 1){
      draw_Min_EINE(colors[1]);
      draw_MINUTE(colors[1]);
      draw_NACH(colors[1]);
    }
    if((Min >= 2) && (Min <= 39)){
      if(Min == 2){
        draw_Min_ZWEI(colors[1]);
      }
      if(Min == 3){
        draw_Min_DREI(colors[1]);
      }
      if(Min == 4){
        draw_Min_VIER(colors[1]);
      }
      if(Min == 5){
        draw_Min_FUENF(colors[1]);
      }
      if(Min == 6){
        draw_Min_SECHS(colors[1]);
      }
      if(Min == 7){
        draw_Min_SIEBEN(colors[1]);
      }
      if(Min == 8){
        draw_Min_ACHT(colors[1]);
      }
      if(Min == 9){
        draw_Min_NEUN(colors[1]);
      }
      if(Min == 10){
        draw_Min_ZEHN(colors[1]);
      }
      if(Min == 11){
        draw_Min_ELF(colors[1]);
      }
      if(Min == 12){
        draw_Min_ZWOELF(colors[1]);
      }
      if(Min == 13){
        draw_Min_DREI(colors[1]);
        draw_Min_ZEHN(colors[1]);
      }
      if(Min == 14){
        draw_Min_VIER(colors[1]);
        draw_Min_ZEHN(colors[1]);
      }
      if(Min == 15){
        draw_Min_FUENF(colors[1]);
        draw_Min_ZEHN(colors[1]);
      }
      if(Min == 16){
        draw_Min_SECHS(colors[1]);
        draw_Min_ZEHN(colors[1]);
      }
      if(Min == 17){
        draw_Min_SIEB(colors[1]);
        draw_Min_ZEHN(colors[1]);
      }
      if(Min == 18){
        draw_Min_ACHT(colors[1]);
        draw_Min_ZEHN(colors[1]);
      }
      if(Min == 19){
        draw_Min_NEUN(colors[1]);
        draw_Min_ZEHN(colors[1]);
      }
      if(Min == 20){
        draw_Min_ZWANZIG(colors[1]);
      }
      if(Min == 21){
        draw_Min_EIN(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_ZWANZIG(colors[1]);
      }
      if(Min == 22){
        draw_Min_ZWEI(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_ZWANZIG(colors[1]);
      }
      if(Min == 23){
        draw_Min_DREI(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_ZWANZIG(colors[1]);
      }
      if(Min == 24){
        draw_Min_VIER(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_ZWANZIG(colors[1]);
      }
      if(Min == 25){
        draw_Min_FUENF(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_ZWANZIG(colors[1]);
      }
      if(Min == 26){
        draw_Min_SECHS(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_ZWANZIG(colors[1]);
      }
      if(Min == 27){
        draw_Min_SIEBEN(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_ZWANZIG(colors[1]);
      }
      if(Min == 28){
        draw_Min_ACHT(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_ZWANZIG(colors[1]);
      }
      if(Min == 29){
        draw_Min_NEUN(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_ZWANZIG(colors[1]);
      }
      if(Min == 30){
        draw_Min_DREISSIG(colors[1]);
      }
      if(Min == 31){
        draw_Min_EIN(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_DREISSIG(colors[1]);
      }
      if(Min == 32){
        draw_Min_ZWEI(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_DREISSIG(colors[1]);
      }
      if(Min == 33){
        draw_Min_DREI(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_DREISSIG(colors[1]);
      }
      if(Min == 34){
        draw_Min_VIER(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_DREISSIG(colors[1]);
      }
      if(Min == 35){
        draw_Min_FUENF(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_DREISSIG(colors[1]);
      }
      if(Min == 36){
        draw_Min_SECHS(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_DREISSIG(colors[1]);
      }
      if(Min == 37){
        draw_Min_SIEBEN(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_DREISSIG(colors[1]);
      }
      if(Min == 38){
        draw_Min_ACHT(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_DREISSIG(colors[1]);
      }
      if(Min == 39){
        draw_Min_NEUN(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_DREISSIG(colors[1]);
      }
      if(Min == 40){
        draw_Min_VIERZIG(colors[1]);
      }
      if(Min == 41){
        draw_Min_EIN(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_VIERZIG(colors[1]);
      }
      if(Min == 42){
        draw_Min_ZWEI(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_VIERZIG(colors[1]);
      }
      if(Min == 43){
        draw_Min_DREI(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_VIERZIG(colors[1]);
      }
      if(Min == 44){
        draw_Min_VIER(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_VIERZIG(colors[1]);
      }
      if(Min == 45){
        draw_Min_FUENF(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_VIERZIG(colors[1]);
      }
      if(Min == 46){
        draw_Min_SECHS(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_VIERZIG(colors[1]);
      }
      if(Min == 47){
        draw_Min_SIEBEN(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_VIERZIG(colors[1]);
      }
      if(Min == 48){
        draw_Min_ACHT(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_VIERZIG(colors[1]);
      }
      if(Min == 49){
        draw_Min_NEUN(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_VIERZIG(colors[1]);
      }
      if(Min == 50){
        draw_Min_FUENFZIG(colors[1]);
      }
      if(Min == 51){
        draw_Min_EIN(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_FUENFZIG(colors[1]);
      }
      if(Min == 52){
        draw_Min_ZWEI(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_FUENFZIG(colors[1]);
      }
      if(Min == 53){
        draw_Min_DREI(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_FUENFZIG(colors[1]);
      }
      if(Min == 54){
        draw_Min_VIER(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_FUENFZIG(colors[1]);
      }
      if(Min == 55){
        draw_Min_FUENF(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_FUENFZIG(colors[1]);
      }
      if(Min == 56){
        draw_Min_SECHS(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_FUENFZIG(colors[1]);
      }
      if(Min == 57){
        draw_Min_SIEBEN(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_FUENFZIG(colors[1]);
      }
      if(Min == 58){
        draw_Min_ACHT(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_FUENFZIG(colors[1]);
      }
      if(Min == 59){
        draw_Min_NEUN(colors[1]);
        draw_Min_UND(colors[1]);
        draw_Min_FUENFZIG(colors[1]);
      }

      draw_MINUTEN(colors[1]);
      draw_NACH(colors[1]);
    }
  }
}

/// the setup function runs once when you press reset or power the board
void setup() {

  Wire.begin();  // Begin I2C
  RTC.begin();   // begin clock
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(16);
  matrix.setTextColor(colors[1]);

  State.Mode = MODE_WORD_CLOCK;
  //State.Mode = MODE_MARQUEE_TIME;
  State.last_Color = colors[1];
  State.Color_Mode = COLOR_MODE_MONO;
  State.Color = colors[1];
}

uint16_t getColor(){
  uint16_t color = 0;

  switch(State.Color_Mode){
    case COLOR_MODE_MONO:
      color = State.Color;
      break;

     case COLOR_MODE_MIXED:
      color = colors[rand()%MAX_COLORS];
      break;

    default: 
      color = 0;

  }


  State.last_Color = color;
  return color;
}

int x    = matrix.width();
int pass = 0;

// the loop function runs over and over again forever
void loop() {
  State.Now = RTC.now();
  resetGrid();

  if(State.Mode == MODE_WORD_CLOCK){
    draw_ES_IST(colors[1]);



  }
  /* Lauftext*/
  if(State.Mode == MODE_MARQUEE_TIME){
    String dataString = "";
    dataString += String(State.Now.hour());
    dataString += ":";
    dataString += String(State.Now.minute());
    dataString += ":";
    dataString += String(State.Now.second());
    matrix.fillScreen(0);
    matrix.setCursor(x, 2);
    //matrix.print(F("Emil"));
    matrix.print(dataString);
    if(--x < -48) {
      x = matrix.width();
      if(++pass >= 7) pass = 0;
      matrix.setTextColor(colors[pass]);
    }
    matrix.show();
  delay(150);
  }else{
    for(int i = 0; i < WIDTH; i++) {
      for(int j = 0; j < HEIGHT; j++) {
        if (State.grid[i][j] != 0) {
          matrix.drawPixel(i, j, State.grid[i][j]);
        }
        else {
          matrix.drawPixel(i, j, matrix.Color(0, 0, 0));
        }

      }
    }
    matrix.show();
  delay(50);
  }
  

/*
  digitalWrite(LED_BUILTIN, HIGH);   // Arduino: turn the LED on (HIGH)
                                     // D1 Mini: turns the LED *off*
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // Arduino: turn the LED off (LOW)
                                     // D1 Mini: turns the LED *on*
  delay(1000);                      // wait for a second
*/

}
