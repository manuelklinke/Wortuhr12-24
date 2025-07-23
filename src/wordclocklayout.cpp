/* V1
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
/* V2
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
0  E  S  B  I  S  T  E  I  N  Z  E  I  T  D  R  E  I  V  I  E  R  T  E  L
1  Z  W  Ö  L  F  Ü  N  F  Ü  R  N  E  U  N  A  C  H  T  E  L  F  V  O  R
2  Z  W  E  I  N  S  E  C  H  S  I  E  B  E  N  E  I  N  E  Z  E  H  N  Y
3  B  I  N  W  O  C  H  E  N  E  N  D  E  N  O  C  H  A  L  B  I  E  R  E
4  S  T  U  N  D  E  N  U  L  L  A  U  H  R  S  P  Ä  T  E  R  U  N  D  U
5  D  R  E  I  N  E  R  V  I  E  R  S  E  C  H  S  I  E  B  E  N  E  U  N
6  Q  F  Ü  N  F  A  C  H  T  Z  E  H  N  I  C  H  T  S  Z  W  E  I  S  T
7  Z  U  N  D  E  L  F  Ü  N  F  Z  I  G  U  T  E  N  V  I  E  R  Z  I  G
8  D  R  E  I  ß  I  G  Z  W  A  N  Z  I  G  Z  W  Ö  L  F  R  Ü  H  E  R
9  I  M  I  N  U  T  E  N  I  X  T  A  G  E  N  B  I  S  K  A  F  F  E  E
10 Z  U  M  O  R  G  E  N  I  E  J  A  V  O  R  M  I  T  T  A  G  R  A  D
11 U  R  L  A  U  B  K  A  L  T  W  A  R  M  I  P  R  Ä  A  B  E  N  D  A
*/

#include "wordclocklayout.h"
#include "wordclock.h"

void draw_ES_IST(uint16_t color, stateType_t *state){
  // ES
  state->grid[0][0] = color;
  state->grid[1][0] = color;
  //IST
  state->grid[3][0] = color;
  state->grid[4][0] = color;
  state->grid[5][0] = color;
}

#ifdef LAYOUT1
void draw_Min_EIN(uint16_t color, stateType_t *state){
  state->grid[2][1] = color;
  state->grid[3][1] = color;
  state->grid[4][1] = color;
}

void draw_Min_EINE(uint16_t color, stateType_t *state){
  draw_Min_EIN(color, state);
  state->grid[5][1] = color;
}

void draw_Min_EINER(uint16_t color, stateType_t *state){
  draw_Min_EINE(color, state);
  state->grid[6][1] = color;
}

void draw_Min_ZWEI(uint16_t color, stateType_t *state){
  state->grid[6][2] = color;
  state->grid[7][2] = color;
  state->grid[8][2] = color;
  state->grid[9][2] = color;
}

void draw_Min_DREI(uint16_t color, stateType_t *state){
  state->grid[0][1] = color;
  state->grid[1][1] = color;
  state->grid[2][1] = color;
  state->grid[3][1] = color;
}

void draw_Min_VIER(uint16_t color, stateType_t *state){
  state->grid[17][1] = color;
  state->grid[18][1] = color;
  state->grid[19][1] = color;
  state->grid[20][1] = color;
}

void draw_Min_FUENF(uint16_t color, stateType_t *state){
  state->grid[18][0] = color;
  state->grid[19][0] = color;
  state->grid[20][0] = color;
  state->grid[21][0] = color;
}

void draw_Min_SECHS(uint16_t color, stateType_t *state){
  state->grid[7][1] = color;
  state->grid[8][1] = color;
  state->grid[9][1] = color;
  state->grid[10][1] = color;
  state->grid[11][1] = color;
}

void draw_Min_SIEB(uint16_t color, stateType_t *state){
  state->grid[11][1] = color;
  state->grid[12][1] = color;
  state->grid[13][1] = color;
  state->grid[14][1] = color;
}

void draw_Min_SIEBEN(uint16_t color, stateType_t *state){
  draw_Min_SIEB(color, state);
  state->grid[15][1] = color;
  state->grid[16][1] = color;
}

void draw_Min_ACHT(uint16_t color, stateType_t *state){
  state->grid[14][2] = color;
  state->grid[15][2] = color;
  state->grid[16][2] = color;
  state->grid[17][2] = color;
}

void draw_Min_NEUN(uint16_t color, stateType_t *state){
  state->grid[10][2] = color;
  state->grid[11][2] = color;
  state->grid[12][2] = color;
  state->grid[13][2] = color;
}

void draw_Min_ZEHN(uint16_t color, stateType_t *state){
  state->grid[0][4] = color;
  state->grid[1][4] = color;
  state->grid[2][4] = color;
  state->grid[3][4] = color;
}

void draw_Min_ELF(uint16_t color, stateType_t *state){
  state->grid[4][3] = color;
  state->grid[5][3] = color;
  state->grid[6][3] = color;
}

void draw_Min_ZWOELF(uint16_t color, stateType_t *state){
  state->grid[7][4] = color;
  state->grid[8][4] = color;
  state->grid[9][4] = color;
  state->grid[10][4] = color;
}

void draw_Min_UND(uint16_t color, stateType_t *state){
  state->grid[1][3] = color;
  state->grid[2][3] = color;
  state->grid[3][3] = color;
}

void draw_Min_ZWANZIG(uint16_t color, stateType_t *state){
  state->grid[0][5] = color;
  state->grid[1][5] = color;
  state->grid[2][5] = color;
  state->grid[3][5] = color;
  state->grid[4][5] = color;
  state->grid[5][5] = color;
  state->grid[6][5] = color;
}

void draw_Min_DREISSIG(uint16_t color, stateType_t *state){
  state->grid[17][4] = color;
  state->grid[18][4] = color;
  state->grid[19][4] = color;
  state->grid[20][4] = color;
  state->grid[21][4] = color;
  state->grid[22][4] = color;
  state->grid[23][4] = color;
}

void draw_Min_VIERZIG(uint16_t color, stateType_t *state){
  state->grid[13][3] = color;
  state->grid[14][3] = color;
  state->grid[15][3] = color;
  state->grid[16][3] = color;
  state->grid[17][3] = color;
  state->grid[18][3] = color;
  state->grid[19][3] = color;
}

void draw_Min_FUENFZIG(uint16_t color, stateType_t *state){
  state->grid[6][3] = color;
  state->grid[7][3] = color;
  state->grid[8][3] = color;
  state->grid[9][3] = color;
  state->grid[10][3] = color;
  state->grid[11][3] = color;
  state->grid[12][3] = color;
}

void draw_MINUTE(uint16_t color, stateType_t *state){
  state->grid[8][5] = color;
  state->grid[9][5] = color;
  state->grid[10][5] = color;
  state->grid[11][5] = color;
  state->grid[12][5] = color;
  state->grid[13][5] = color;
}

void draw_MINUTEN(uint16_t color, stateType_t *state){
  draw_MINUTE(color, state);
  state->grid[14][5] = color;
}

void draw_VOR(uint16_t color, stateType_t *state){
  state->grid[21][5] = color;
  state->grid[22][5] = color;
  state->grid[23][5] = color;
}

void draw_NACH(uint16_t color, stateType_t *state){
  state->grid[17][5] = color;
  state->grid[18][5] = color;
  state->grid[19][5] = color;
  state->grid[20][5] = color;
}

void draw_HALB(uint16_t color, stateType_t *state){
  state->grid[0][6] = color;
  state->grid[1][6] = color;
  state->grid[2][6] = color;
  state->grid[3][6] = color;
}

void draw_VIERTEL(uint16_t color, stateType_t *state){
  draw_Min_VIER(color, state);
  state->grid[21][1] = color;
  state->grid[22][1] = color;
  state->grid[23][1] = color;
}

void draw_Std_NULL(uint16_t color, stateType_t *state){
  state->grid[3][4] = color;
  state->grid[4][4] = color;
  state->grid[5][4] = color;
  state->grid[6][4] = color;
}

void draw_Std_EIN(uint16_t color, stateType_t *state){
  state->grid[10][8] = color;
  state->grid[11][8] = color;
  state->grid[12][8] = color;
}

void draw_Std_EINS(uint16_t color, stateType_t *state){
  draw_Std_EIN(color, state);
  state->grid[13][8] = color;
}

void draw_Std_ZWEI(uint16_t color, stateType_t *state){
  state->grid[18][8] = color;
  state->grid[19][8] = color;
  state->grid[20][8] = color;
  state->grid[21][8] = color;
}

void draw_Std_DREI(uint16_t color, stateType_t *state){
  state->grid[0][7] = color;
  state->grid[1][7] = color;
  state->grid[2][7] = color;
  state->grid[3][7] = color;
}

void draw_Std_VIER(uint16_t color, stateType_t *state){
  state->grid[4][7] = color;
  state->grid[5][7] = color;
  state->grid[6][7] = color;
  state->grid[7][7] = color;
}

void draw_Std_FUENF(uint16_t color, stateType_t *state){
  state->grid[11][7] = color;
  state->grid[12][7] = color;
  state->grid[13][7] = color;
  state->grid[14][7] = color;
}

void draw_Std_SECHS(uint16_t color, stateType_t *state){
  state->grid[0][8] = color;
  state->grid[1][8] = color;
  state->grid[2][8] = color;
  state->grid[3][8] = color;
  state->grid[4][8] = color;
}

void draw_Std_SIEBEN(uint16_t color, stateType_t *state){
  state->grid[4][8] = color;
  state->grid[5][8] = color;
  state->grid[6][8] = color;
  state->grid[7][8] = color;
  state->grid[8][8] = color;
  state->grid[9][8] = color;
}

void draw_Std_ACHT(uint16_t color, stateType_t *state){
  state->grid[19][7] = color;
  state->grid[20][7] = color;
  state->grid[21][7] = color;
  state->grid[22][7] = color;
}

void draw_Std_NEUN(uint16_t color, stateType_t *state){
  state->grid[15][7] = color;
  state->grid[16][7] = color;
  state->grid[17][7] = color;
  state->grid[18][7] = color;
}

void draw_Std_ZEHN(uint16_t color, stateType_t *state){
  state->grid[14][8] = color;
  state->grid[15][8] = color;
  state->grid[16][8] = color;
  state->grid[17][8] = color;
}

void draw_Std_ELF(uint16_t color, stateType_t *state){
  state->grid[9][7] = color;
  state->grid[10][7] = color;
  state->grid[11][7] = color;
}

void draw_Std_ZWOELF(uint16_t color, stateType_t *state){
  state->grid[0][9] = color;
  state->grid[1][9] = color;
  state->grid[2][9] = color;
  state->grid[3][9] = color;
  state->grid[4][9] = color;
}

void draw_UHR1(uint16_t color, stateType_t *state){
  state->grid[11][9] = color;
  state->grid[12][9] = color;
  state->grid[13][9] = color;
}

void draw_UHR2(uint16_t color, stateType_t *state){
  state->grid[11][11] = color;
  state->grid[12][11] = color;
  state->grid[13][11] = color;
}

void draw_MORGEN(uint16_t color, stateType_t *state){
  state->grid[1][10] = color;
  state->grid[2][10] = color;
  state->grid[3][10] = color;
  state->grid[4][10] = color;
  state->grid[5][10] = color;
  state->grid[6][10] = color;
}

void draw_MORGENS(uint16_t color, stateType_t *state){
  draw_MORGEN(color, state);
  state->grid[7][10] = color;
}

void draw_MITTAG(uint16_t color, stateType_t *state){
  state->grid[11][10] = color;
  state->grid[12][10] = color;
  state->grid[13][10] = color;
  state->grid[14][10] = color;
  state->grid[13][10] = color;
  state->grid[14][10] = color;
}

void draw_MITTAGS(uint16_t color, stateType_t *state){
  draw_MITTAG(color, state);
  state->grid[15][10] = color;
}

void draw_VORMITTAG(uint16_t color, stateType_t *state){
  state->grid[8][10] = color;
  state->grid[9][10] = color;
  state->grid[10][10] = color;
  draw_MITTAG(color, state);
}

void draw_VORMITTAGS(uint16_t color, stateType_t *state){
  draw_VORMITTAG(color, state);
  state->grid[11][10] = color;
}

void draw_ABEND(uint16_t color, stateType_t *state){
  state->grid[18][9] = color;
  state->grid[19][9] = color;
  state->grid[20][9] = color;
  state->grid[21][9] = color;
  state->grid[22][9] = color;
}

void draw_ABENDS(uint16_t color, stateType_t *state){
  draw_ABEND(color, state);
  state->grid[23][9] = color;
}

void draw_PRAEABEND(uint16_t color, stateType_t *state){
  state->grid[15][9] = color;
  state->grid[16][9] = color;
  state->grid[17][9] = color;
  draw_ABEND(color, state);
}

void draw_PRAEABENDS(uint16_t color, stateType_t *state){
  draw_PRAEABEND(color, state);
  state->grid[23][9] = color;
}

void draw_NACHT(uint16_t color, stateType_t *state){
  state->grid[18][7] = color;
  draw_Std_ACHT(color, state);
}

void draw_NACHTS(uint16_t color, stateType_t *state){
  draw_NACHT(color, state);
  state->grid[23][7] = color;
}

void draw_ZEIT(uint16_t color, stateType_t *state){
  state->grid[14][0] = color;
  state->grid[15][0] = color;
  state->grid[16][0] = color;
  state->grid[17][0] = color;
}

void draw_TAG(uint16_t color, stateType_t *state){
  state->grid[17][6] = color;
  state->grid[18][6] = color;
  state->grid[19][6] = color;
}

void draw_TAGE(uint16_t color, stateType_t *state){
  draw_TAG(color, state);
  state->grid[20][6] = color;
}

void draw_TAGEN(uint16_t color, stateType_t *state){
  draw_TAGE(color, state);
  state->grid[21][6] = color;
}

void draw_WOCHE(uint16_t color, stateType_t *state){
  state->grid[7][6] = color;
  state->grid[8][6] = color;
  state->grid[9][6] = color;
  state->grid[10][6] = color;
  state->grid[11][6] = color;
}

void draw_WOCHEN(uint16_t color, stateType_t *state){
  draw_WOCHE(color, state);
  state->grid[12][6] = color;
}

void draw_ENDE(uint16_t color, stateType_t *state){
  state->grid[13][6] = color;
  state->grid[14][6] = color;
  state->grid[15][6] = color;
  state->grid[16][6] = color;
}

void draw_WOCHENENDE(uint16_t color, stateType_t *state){
  draw_WOCHEN(color, state);
  draw_ENDE(color, state);
}

void draw_NIE(uint16_t color, stateType_t *state){
  state->grid[21][6] = color;
  state->grid[22][6] = color;
  state->grid[23][6] = color;
}

void draw_GENIE(uint16_t color, stateType_t *state){
  state->grid[19][6] = color;
  state->grid[20][6] = color;
  draw_NIE(color, state);
}

void draw_NIX(uint16_t color, stateType_t *state){
  state->grid[14][5] = color;
  state->grid[15][5] = color;
  state->grid[16][5] = color;
}

void draw_VIEL(uint16_t color, stateType_t *state){
  state->grid[8][0] = color;
  state->grid[9][0] = color;
  state->grid[10][0] = color;
  state->grid[11][0] = color;
}

void draw_VIELE(uint16_t color, stateType_t *state){
  draw_VIEL(color, state);
  state->grid[12][0] = color;
}

void draw_WENIG(uint16_t color, stateType_t *state){
  state->grid[0][2] = color;
  state->grid[1][2] = color;
  state->grid[2][2] = color;
  state->grid[3][2] = color;
  state->grid[4][2] = color;
}

void draw_WENIGE(uint16_t color, stateType_t *state){
  draw_WENIG(color, state);
  state->grid[5][2] = color;
}

void draw_SPAET(uint16_t color, stateType_t *state){
  state->grid[18][2] = color;
  state->grid[19][2] = color;
  state->grid[20][2] = color;
  state->grid[21][2] = color;
}

void draw_SPAETER(uint16_t color, stateType_t *state){
  draw_SPAET(color, state);
  state->grid[22][2] = color;
  state->grid[23][2] = color;
}

void draw_FUER(uint16_t color, stateType_t *state){
  state->grid[21][0] = color;
  state->grid[22][0] = color;
  state->grid[23][0] = color;
}

void draw_KAFFEE(uint16_t color, stateType_t *state){
  state->grid[5][9] = color;
  state->grid[6][9] = color;
  state->grid[7][9] = color;
  state->grid[8][9] = color;
  state->grid[9][9] = color;
  state->grid[10][9] = color;
}

void draw_EIS(uint16_t color, stateType_t *state){
  state->grid[20][8] = color;
  state->grid[21][8] = color;
  state->grid[22][8] = color;
}

void draw_BIER(uint16_t color, stateType_t *state){
  state->grid[3][6] = color;
  state->grid[4][6] = color;
  state->grid[5][6] = color;
  state->grid[6][6] = color;
}

void draw_WEIN(uint16_t color, stateType_t *state){
  state->grid[7][2] = color;
  state->grid[8][2] = color;
  state->grid[9][2] = color;
  state->grid[10][2] = color;
}

void draw_GUT(uint16_t color, stateType_t *state){
  state->grid[19][3] = color;
  state->grid[20][3] = color;
  state->grid[21][3] = color;
}

void draw_GUTE(uint16_t color, stateType_t *state){
  draw_GUT(color, state);
  state->grid[22][3] = color;
}

void draw_GUTEN(uint16_t color, stateType_t *state){
  draw_GUTE(color, state);
  state->grid[23][3] = color;
}

void draw_NEIN(uint16_t color, stateType_t *state){
  state->grid[9][8] = color;
  state->grid[10][8] = color;
  state->grid[11][8] = color;
  state->grid[12][8] = color;
}

void draw_GRAD(uint16_t color, stateType_t *state){
  state->grid[0][11] = color;
  state->grid[1][11] = color;
  state->grid[2][11] = color;
  state->grid[3][11] = color;
}

void draw_URLAUB(uint16_t color, stateType_t *state){
  state->grid[4][11] = color;
  state->grid[5][11] = color;
  state->grid[6][11] = color;
  state->grid[7][11] = color;
  state->grid[8][11] = color;
  state->grid[9][11] = color;
}

void draw_ZU(uint16_t color, stateType_t *state){
  state->grid[10][11] = color;
  state->grid[11][11] = color;
}

void draw_WARM(uint16_t color, stateType_t *state){
  state->grid[14][11] = color;
  state->grid[15][11] = color;
  state->grid[16][11] = color;
  state->grid[17][11] = color;
}

void draw_KALT(uint16_t color, stateType_t *state){
  state->grid[20][11] = color;
  state->grid[21][11] = color;
  state->grid[22][11] = color;
  state->grid[23][11] = color;
}

void draw_MI1(uint16_t color, stateType_t *state){
  state->grid[8][5] = color;
  state->grid[9][5] = color;
}

void draw_MI2(uint16_t color, stateType_t *state){
  state->grid[11][10] = color;
  state->grid[12][10] = color;
}

void draw_MI3(uint16_t color, stateType_t *state){
  state->grid[17][11] = color;
  state->grid[18][11] = color;
}

void draw_MIMIMI(uint16_t color, stateType_t *state){
  draw_MI1(color, state);
  draw_MI2(color, state);
  draw_MI3(color, state);
}

void draw_IN(uint16_t color, stateType_t *state){
  state->grid[6][0] = color;
  state->grid[7][0] = color;
}

void draw_IST2(uint16_t color, stateType_t *state){
  state->grid[21][8] = color;
  state->grid[22][8] = color;
  state->grid[23][8] = color;
}

void draw_AM(uint16_t color, stateType_t *state){
  state->grid[7][5] = color;
  state->grid[8][5] = color;
}

#endif

#ifdef LAYOUT2

void draw_EIN1(uint16_t color, stateType_t *state){
  state->grid[6][0] = color;
  state->grid[7][0] = color;
  state->grid[8][0] = color;
}

void draw_STEIN(uint16_t color, stateType_t *state){
  state->grid[4][5] = color;
  state->grid[5][5] = color;
  draw_EIN1(color, state);
}

void draw_Std_NULL(uint16_t color, stateType_t *state){
  state->grid[6][4] = color;
  state->grid[7][4] = color;
  state->grid[8][4] = color;
  state->grid[9][4] = color;
}

void draw_Std_EIN(uint16_t color, stateType_t *state){
  state->grid[2][2] = color;
  state->grid[3][2] = color;
  state->grid[4][2] = color;
}

void draw_Std_EINS(uint16_t color, stateType_t *state){
  draw_Std_EIN(color, state);
  state->grid[5][2] = color;
}

void draw_Std_EIN2(uint16_t color, stateType_t *state){
  state->grid[15][2] = color;
  state->grid[16][2] = color;
  state->grid[17][2] = color;
}

void draw_Std_EINE(uint16_t color, stateType_t *state){
  draw_Std_EIN2(color, state);
  state->grid[18][2] = color;
}

void draw_Std_ZWEI(uint16_t color, stateType_t *state){
  state->grid[0][2] = color;
  state->grid[1][2] = color;
  state->grid[2][2] = color;
  state->grid[3][2] = color;
}

void draw_Std_DREI(uint16_t color, stateType_t *state){
  state->grid[13][0] = color;
  state->grid[14][0] = color;
  state->grid[15][0] = color;
  state->grid[16][0] = color;
}

void draw_Std_VIER(uint16_t color, stateType_t *state){
  state->grid[17][0] = color;
  state->grid[18][0] = color;
  state->grid[19][0] = color;
  state->grid[20][0] = color;
}

void draw_VIERTEL(uint16_t color, stateType_t *state){
  draw_Std_VIER(color, state);
  state->grid[21][0] = color;
  state->grid[22][0] = color;
  state->grid[23][0] = color;
}

void draw_Std_FUENF(uint16_t color, stateType_t *state){
  state->grid[4][1] = color;
  state->grid[5][1] = color;
  state->grid[6][1] = color;
  state->grid[7][1] = color;
}

void draw_Std_SECHS(uint16_t color, stateType_t *state){
  state->grid[5][2] = color;
  state->grid[6][2] = color;
  state->grid[7][2] = color;
  state->grid[8][2] = color;
  state->grid[9][2] = color;
}

void draw_Std_SIEBEN(uint16_t color, stateType_t *state){
  state->grid[9][2] = color;
  state->grid[10][2] = color;
  state->grid[11][2] = color;
  state->grid[12][2] = color;
  state->grid[13][2] = color;
  state->grid[14][2] = color;
}

void draw_Std_ACHT(uint16_t color, stateType_t *state){
  state->grid[14][1] = color;
  state->grid[15][1] = color;
  state->grid[16][1] = color;
  state->grid[17][1] = color;
}

void draw_Std_NEUN(uint16_t color, stateType_t *state){
  state->grid[10][1] = color;
  state->grid[11][1] = color;
  state->grid[12][1] = color;
  state->grid[13][1] = color;
}

void draw_Std_ZEHN(uint16_t color, stateType_t *state){
  state->grid[19][2] = color;
  state->grid[20][2] = color;
  state->grid[21][2] = color;
  state->grid[22][2] = color;
}

void draw_Std_ELF(uint16_t color, stateType_t *state){
  state->grid[18][1] = color;
  state->grid[19][1] = color;
  state->grid[20][1] = color;
}

void draw_Std_ZWOELF(uint16_t color, stateType_t *state){
  state->grid[0][1] = color;
  state->grid[1][1] = color;
  state->grid[2][1] = color;
  state->grid[3][1] = color;
  state->grid[4][1] = color;
}

void draw_Std_UND(uint16_t color, stateType_t *state){
  state->grid[20][4] = color;
  state->grid[21][4] = color;
  state->grid[22][4] = color;
}

void draw_VOR(uint16_t color, stateType_t *state){
  state->grid[21][1] = color;
  state->grid[22][1] = color;
  state->grid[23][1] = color;
}

void draw_NACH(uint16_t color, stateType_t *state){
  state->grid[13][1] = color;
  state->grid[14][1] = color;
  state->grid[15][1] = color;
  state->grid[16][1] = color;
}

void draw_HALB(uint16_t color, stateType_t *state){
  state->grid[16][3] = color;
  state->grid[17][3] = color;
  state->grid[18][3] = color;
  state->grid[19][3] = color;
}

void draw_UHR1(uint16_t color, stateType_t *state){
  state->grid[11][4] = color;
  state->grid[12][4] = color;
  state->grid[13][4] = color;
}

void draw_Min_EIN(uint16_t color, stateType_t *state){
  state->grid[2][5] = color;
  state->grid[3][5] = color;
  state->grid[4][5] = color;
}

void draw_Min_EINE(uint16_t color, stateType_t *state){
  draw_Min_EIN(color, state);
  state->grid[5][5] = color;
}

void draw_Min_EINER(uint16_t color, stateType_t *state){
  draw_Min_EINE(color, state);
  state->grid[6][5] = color;
}

void draw_Min_ZWEI(uint16_t color, stateType_t *state){
  state->grid[18][6] = color;
  state->grid[19][6] = color;
  state->grid[20][6] = color;
  state->grid[21][6] = color;
}

void draw_Min_DREI(uint16_t color, stateType_t *state){
  state->grid[0][5] = color;
  state->grid[1][5] = color;
  state->grid[2][5] = color;
  state->grid[3][5] = color;
}

void draw_Min_VIER(uint16_t color, stateType_t *state){
  state->grid[7][5] = color;
  state->grid[8][5] = color;
  state->grid[9][5] = color;
  state->grid[10][5] = color;
}

void draw_Min_FUENF(uint16_t color, stateType_t *state){
  state->grid[1][6] = color;
  state->grid[2][6] = color;
  state->grid[3][6] = color;
  state->grid[4][6] = color;
}

void draw_Min_SECHS(uint16_t color, stateType_t *state){
  state->grid[11][5] = color;
  state->grid[12][5] = color;
  state->grid[13][5] = color;
  state->grid[14][5] = color;
  state->grid[15][5] = color;
}

void draw_Min_SIEB(uint16_t color, stateType_t *state){
  state->grid[15][5] = color;
  state->grid[16][5] = color;
  state->grid[17][5] = color;
  state->grid[18][5] = color;
}

void draw_Min_SIEBEN(uint16_t color, stateType_t *state){
  draw_Min_SIEB(color, state);
  state->grid[19][5] = color;
  state->grid[20][5] = color;
}

void draw_Min_ACHT(uint16_t color, stateType_t *state){
  state->grid[5][6] = color;
  state->grid[6][6] = color;
  state->grid[7][6] = color;
  state->grid[8][6] = color;
}

void draw_Min_NEUN(uint16_t color, stateType_t *state){
  state->grid[20][5] = color;
  state->grid[21][5] = color;
  state->grid[22][5] = color;
  state->grid[23][5] = color;
}

void draw_Min_ZEHN(uint16_t color, stateType_t *state){
  state->grid[9][6] = color;
  state->grid[10][6] = color;
  state->grid[11][6] = color;
  state->grid[12][6] = color;
}

void draw_Min_ELF(uint16_t color, stateType_t *state){
  state->grid[4][7] = color;
  state->grid[5][7] = color;
  state->grid[6][7] = color;
}

void draw_Min_ZWOELF(uint16_t color, stateType_t *state){
  state->grid[14][8] = color;
  state->grid[15][8] = color;
  state->grid[16][8] = color;
  state->grid[17][8] = color;
  state->grid[18][8] = color;
}

void draw_Min_UND(uint16_t color, stateType_t *state){
  state->grid[1][7] = color;
  state->grid[2][7] = color;
  state->grid[3][7] = color;
}

void draw_Min_ZWANZIG(uint16_t color, stateType_t *state){
  state->grid[7][8] = color;
  state->grid[8][8] = color;
  state->grid[9][8] = color;
  state->grid[10][8] = color;
  state->grid[11][8] = color;
  state->grid[12][8] = color;
  state->grid[13][8] = color;
}

void draw_Min_DREISSIG(uint16_t color, stateType_t *state){
  state->grid[0][8] = color;
  state->grid[1][8] = color;
  state->grid[2][8] = color;
  state->grid[3][8] = color;
  state->grid[4][8] = color;
  state->grid[5][8] = color;
  state->grid[6][8] = color;
}

void draw_Min_VIERZIG(uint16_t color, stateType_t *state){
  state->grid[17][7] = color;
  state->grid[18][7] = color;
  state->grid[19][7] = color;
  state->grid[20][7] = color;
  state->grid[21][7] = color;
  state->grid[22][7] = color;
  state->grid[23][7] = color;
}

void draw_Min_FUENFZIG(uint16_t color, stateType_t *state){
  state->grid[6][7] = color;
  state->grid[7][7] = color;
  state->grid[8][7] = color;
  state->grid[9][7] = color;
  state->grid[10][7] = color;
  state->grid[11][7] = color;
  state->grid[12][7] = color;
}

void draw_MINUTE(uint16_t color, stateType_t *state){
  state->grid[1][9] = color;
  state->grid[2][9] = color;
  state->grid[3][9] = color;
  state->grid[4][9] = color;
  state->grid[5][9] = color;
  state->grid[6][9] = color;
}

void draw_MINUTEN(uint16_t color, stateType_t *state){
  draw_MINUTE(color, state);
  state->grid[7][9] = color;
}

void draw_MORGEN(uint16_t color, stateType_t *state){
  state->grid[2][10] = color;
  state->grid[3][10] = color;
  state->grid[4][10] = color;
  state->grid[5][10] = color;
  state->grid[6][10] = color;
  state->grid[7][10] = color;
}

void draw_MITTAG(uint16_t color, stateType_t *state){
  state->grid[15][10] = color;
  state->grid[16][10] = color;
  state->grid[17][10] = color;
  state->grid[18][10] = color;
  state->grid[19][10] = color;
  state->grid[20][10] = color;
}

void draw_VORMITTAG(uint16_t color, stateType_t *state){
  state->grid[12][10] = color;
  state->grid[13][10] = color;
  state->grid[14][10] = color;
  draw_MITTAG(color, state);
}

void draw_ABEND(uint16_t color, stateType_t *state){
  state->grid[18][11] = color;
  state->grid[19][11] = color;
  state->grid[20][11] = color;
  state->grid[21][11] = color;
  state->grid[22][11] = color;
}

void draw_PRAEABEND(uint16_t color, stateType_t *state){
  state->grid[15][11] = color;
  state->grid[16][11] = color;
  state->grid[17][11] = color;
  draw_ABEND(color, state);
}


void draw_NACHT1(uint16_t color, stateType_t *state){
  state->grid[13][1] = color;
  draw_Std_ACHT(color, state);
}

void draw_NACHT2(uint16_t color, stateType_t *state){
  state->grid[4][6] = color;
  draw_Min_ACHT(color, state);
}

void draw_ZEIT(uint16_t color, stateType_t *state){
  state->grid[9][0] = color;
  state->grid[10][0] = color;
  state->grid[11][0] = color;
  state->grid[12][0] = color;
}

void draw_TAG1(uint16_t color, stateType_t *state){
  state->grid[10][9] = color;
  state->grid[11][9] = color;
  state->grid[12][9] = color;
}

void draw_TAGE(uint16_t color, stateType_t *state){
  draw_TAG1(color, state);
  state->grid[13][9] = color;
}

void draw_TAGEN(uint16_t color, stateType_t *state){
  draw_TAGE(color, state);
  state->grid[14][9] = color;
}

void draw_TAG2(uint16_t color, stateType_t *state){
  state->grid[18][10] = color;
  state->grid[19][10] = color;
  state->grid[20][10] = color;
}

void draw_WOCHE(uint16_t color, stateType_t *state){
  state->grid[3][3] = color;
  state->grid[4][3] = color;
  state->grid[5][3] = color;
  state->grid[6][3] = color;
  state->grid[7][3] = color;
}

void draw_WOCHEN(uint16_t color, stateType_t *state){
  draw_WOCHE(color, state);
  state->grid[8][3] = color;
}

void draw_ENDE(uint16_t color, stateType_t *state){
  state->grid[9][3] = color;
  state->grid[10][3] = color;
  state->grid[11][3] = color;
  state->grid[12][3] = color;
}

void draw_WOCHENENDE(uint16_t color, stateType_t *state){
  draw_WOCHEN(color, state);
  draw_ENDE(color, state);
}

void draw_NIE(uint16_t color, stateType_t *state){
  state->grid[7][10] = color;
  state->grid[8][10] = color;
  state->grid[9][10] = color;
}

void draw_GENIE(uint16_t color, stateType_t *state){
  state->grid[5][10] = color;
  state->grid[6][10] = color;
  draw_NIE(color, state);
}

void draw_NIX(uint16_t color, stateType_t *state){
  state->grid[7][9] = color;
  state->grid[8][9] = color;
  state->grid[9][9] = color;
}

void draw_SPAET(uint16_t color, stateType_t *state){
  state->grid[14][4] = color;
  state->grid[15][4] = color;
  state->grid[16][4] = color;
  state->grid[17][4] = color;
}

void draw_SPAETER(uint16_t color, stateType_t *state){
  draw_SPAET(color, state);
  state->grid[18][4] = color;
  state->grid[19][4] = color;
}

void draw_FUER(uint16_t color, stateType_t *state){
  state->grid[7][1] = color;
  state->grid[8][1] = color;
  state->grid[9][1] = color;
}

void draw_KAFFEE(uint16_t color, stateType_t *state){
  state->grid[18][9] = color;
  state->grid[19][9] = color;
  state->grid[20][9] = color;
  state->grid[21][9] = color;
  state->grid[22][9] = color;
  state->grid[23][9] = color;
}

void draw_EIS(uint16_t color, stateType_t *state){
  state->grid[20][6] = color;
  state->grid[21][6] = color;
  state->grid[22][6] = color;
}

void draw_BIER(uint16_t color, stateType_t *state){
  state->grid[19][3] = color;
  state->grid[20][3] = color;
  state->grid[21][3] = color;
  state->grid[22][3] = color;
}

void draw_WEIN(uint16_t color, stateType_t *state){
  state->grid[1][2] = color;
  state->grid[2][2] = color;
  state->grid[3][2] = color;
  state->grid[4][2] = color;
}

void draw_GUT(uint16_t color, stateType_t *state){
  state->grid[12][8] = color;
  state->grid[13][8] = color;
  state->grid[14][8] = color;
}

void draw_GUTE(uint16_t color, stateType_t *state){
  draw_GUT(color, state);
  state->grid[15][8] = color;
}

void draw_GUTEN(uint16_t color, stateType_t *state){
  draw_GUTE(color, state);
  state->grid[16][8] = color;
}

void draw_NEIN1(uint16_t color, stateType_t *state){
  state->grid[14][2] = color;
  state->grid[15][2] = color;
  state->grid[16][2] = color;
  state->grid[17][2] = color;
}

void draw_GRAD(uint16_t color, stateType_t *state){
  state->grid[20][10] = color;
  state->grid[21][10] = color;
  state->grid[22][10] = color;
  state->grid[23][10] = color;
}

void draw_URLAUB(uint16_t color, stateType_t *state){
  state->grid[0][11] = color;
  state->grid[1][11] = color;
  state->grid[2][11] = color;
  state->grid[3][11] = color;
  state->grid[4][11] = color;
  state->grid[5][11] = color;
}

void draw_ZU(uint16_t color, stateType_t *state){
  state->grid[0][10] = color;
  state->grid[1][10] = color;
}

void draw_WARM(uint16_t color, stateType_t *state){
  state->grid[10][11] = color;
  state->grid[11][11] = color;
  state->grid[12][11] = color;
  state->grid[13][11] = color;
}

void draw_KALT(uint16_t color, stateType_t *state){
  state->grid[6][11] = color;
  state->grid[7][11] = color;
  state->grid[8][11] = color;
  state->grid[9][11] = color;
}

void draw_MI1(uint16_t color, stateType_t *state){
  state->grid[1][9] = color;
  state->grid[2][9] = color;
}

void draw_MI2(uint16_t color, stateType_t *state){
  state->grid[15][10] = color;
  state->grid[16][10] = color;
}

void draw_MI3(uint16_t color, stateType_t *state){
  state->grid[13][11] = color;
  state->grid[14][11] = color;
}

void draw_MIMIMI(uint16_t color, stateType_t *state){
  draw_MI1(color, state);
  draw_MI2(color, state);
  draw_MI3(color, state);
}

void draw_IN(uint16_t color, stateType_t *state){
  state->grid[7][0] = color;
  state->grid[8][0] = color;
}

void draw_IST2(uint16_t color, stateType_t *state){
  state->grid[21][6] = color;
  state->grid[22][6] = color;
  state->grid[23][6] = color;
}

void draw_AM(uint16_t color, stateType_t *state){
  state->grid[0][9] = color;
  state->grid[1][9] = color;
}

void draw_BIN(uint16_t color, stateType_t *state){
  state->grid[0][3] = color;
  state->grid[1][3] = color;
  state->grid[2][3] = color;
}

void draw_JA(uint16_t color, stateType_t *state){
  state->grid[10][10] = color;
  state->grid[11][10] = color;
}

void draw_LAU(uint16_t color, stateType_t *state){
  state->grid[9][4] = color;
  state->grid[10][4] = color;
  state->grid[11][4] = color;
}

void draw_NERV(uint16_t color, stateType_t *state){
  state->grid[4][5] = color;
  state->grid[5][5] = color;
  state->grid[6][5] = color;
  state->grid[7][5] = color;
}

void draw_BIS(uint16_t color, stateType_t *state){
  state->grid[15][9] = color;
  state->grid[16][9] = color;
  state->grid[17][9] = color;
}

void draw_NICHT(uint16_t color, stateType_t *state){
  state->grid[12][6] = color;
  state->grid[13][6] = color;
  state->grid[14][6] = color;
  state->grid[15][6] = color;
}

void draw_NICHTS(uint16_t color, stateType_t *state){
  draw_NICHT(color, state);
  state->grid[16][6] = color;
  
}

void draw_DA(uint16_t color, stateType_t *state){
  state->grid[22][11] = color;
  state->grid[23][11] = color;
}

void draw_DU(uint16_t color, stateType_t *state){
  state->grid[22][4] = color;
  state->grid[23][4] = color;
}

void draw_ZUM(uint16_t color, stateType_t *state){
  state->grid[0][10] = color;
  state->grid[1][10] = color;
  state->grid[2][10] = color;
}

#endif