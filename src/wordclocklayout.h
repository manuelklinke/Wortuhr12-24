#ifndef _WORDCLOCKLAYOUT_H
#define _WORDCLOCKLAYOUT_H

#include <wordclock.h>

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

extern void draw_ES_IST(uint16_t color, stateType_t *state);
extern void draw_Min_EIN(uint16_t color, stateType_t *state);
extern void draw_Min_EINE(uint16_t color, stateType_t *state);
extern void draw_Min_EINER(uint16_t color, stateType_t *state);
extern void draw_Min_ZWEI(uint16_t color, stateType_t *state);
extern void draw_Min_DREI(uint16_t color, stateType_t *state);
extern void draw_Min_VIER(uint16_t color, stateType_t *state);
extern void draw_Min_FUENF(uint16_t color, stateType_t *state);
extern void draw_Min_SECHS(uint16_t color, stateType_t *state);
extern void draw_Min_SIEB(uint16_t color, stateType_t *state);
extern void draw_Min_SIEBEN(uint16_t color, stateType_t *state);
extern void draw_Min_ACHT(uint16_t color, stateType_t *state);
extern void draw_Min_NEUN(uint16_t color, stateType_t *state);
extern void draw_Min_ZEHN(uint16_t color, stateType_t *state);
extern void draw_Min_ELF(uint16_t color, stateType_t *state);
extern void draw_Min_ZWOELF(uint16_t color, stateType_t *state);
extern void draw_Min_UND(uint16_t color, stateType_t *state);
extern void draw_Min_ZWANZIG(uint16_t color, stateType_t *state);
extern void draw_Min_DREISSIG(uint16_t color, stateType_t *state);
extern void draw_Min_VIERZIG(uint16_t color, stateType_t *state);
extern void draw_Min_FUENFZIG(uint16_t color, stateType_t *state);

extern void draw_MINUTE(uint16_t color, stateType_t *state);
extern void draw_MINUTEN(uint16_t color, stateType_t *state);
extern void draw_VOR(uint16_t color, stateType_t *state);
extern void draw_NACH(uint16_t color, stateType_t *state);
extern void draw_HALB(uint16_t color, stateType_t *state);
extern void draw_VIERTEL(uint16_t color, stateType_t *state);

extern void draw_Std_NULL(uint16_t color, stateType_t *state);
extern void draw_Std_EIN(uint16_t color, stateType_t *state);
extern void draw_Std_EINS(uint16_t color, stateType_t *state);
extern void draw_Std_ZWEI(uint16_t color, stateType_t *state);
extern void draw_Std_DREI(uint16_t color, stateType_t *state);
extern void draw_Std_VIER(uint16_t color, stateType_t *state);
extern void draw_Std_FUENF(uint16_t color, stateType_t *state);
extern void draw_Std_SECHS(uint16_t color, stateType_t *state);
extern void draw_Std_SIEBEN(uint16_t color, stateType_t *state);
extern void draw_Std_ACHT(uint16_t color, stateType_t *state);
extern void draw_Std_NEUN(uint16_t color, stateType_t *state);
extern void draw_Std_ZEHN(uint16_t color, stateType_t *state);
extern void draw_Std_ELF(uint16_t color, stateType_t *state);
extern void draw_Std_ZWOELF(uint16_t color, stateType_t *state);
extern void draw_Std_UND(uint16_t color, stateType_t *state);

extern void draw_UHR1(uint16_t color, stateType_t *state);
extern void draw_MORGEN(uint16_t color, stateType_t *state);
extern void draw_MITTAG(uint16_t color, stateType_t *state);
extern void draw_MITTAGS(uint16_t color, stateType_t *state);
extern void draw_VORMITTAG(uint16_t color, stateType_t *state);
extern void draw_ABEND(uint16_t color, stateType_t *state);
extern void draw_PRAEABEND(uint16_t color, stateType_t *state);

extern void draw_NACHT(uint16_t color, stateType_t *state);
extern void draw_NACHTS(uint16_t color, stateType_t *state);
extern void draw_ZEIT(uint16_t color, stateType_t *state);
extern void draw_TAG(uint16_t color, stateType_t *state);
extern void draw_TAGE(uint16_t color, stateType_t *state);
extern void draw_TAGEN(uint16_t color, stateType_t *state);
extern void draw_WOCHE(uint16_t color, stateType_t *state);
extern void draw_WOCHEN(uint16_t color, stateType_t *state);
extern void draw_ENDE(uint16_t color, stateType_t *state);
extern void draw_WOCHENENDE(uint16_t color, stateType_t *state);
extern void draw_NIX(uint16_t color, stateType_t *state);


extern void draw_SPAET(uint16_t color, stateType_t *state);
extern void draw_SPAETER(uint16_t color, stateType_t *state);
extern void draw_FUER(uint16_t color, stateType_t *state);
extern void draw_KAFFEE(uint16_t color, stateType_t *state);
extern void draw_EIS(uint16_t color, stateType_t *state);
extern void draw_BIER(uint16_t color, stateType_t *state);
extern void draw_WEIN(uint16_t color, stateType_t *state);
extern void draw_GUT(uint16_t color, stateType_t *state);
extern void draw_GUTE(uint16_t color, stateType_t *state);
extern void draw_GUTEN(uint16_t color, stateType_t *state);
extern void draw_NEIN(uint16_t color, stateType_t *state);
extern void draw_GRAD(uint16_t color, stateType_t *state);
extern void draw_URLAUB(uint16_t color, stateType_t *state);
extern void draw_ZU(uint16_t color, stateType_t *state);
extern void draw_WARM(uint16_t color, stateType_t *state);
extern void draw_KALT(uint16_t color, stateType_t *state);
extern void draw_MI1(uint16_t color, stateType_t *state);
extern void draw_MI2(uint16_t color, stateType_t *state);
extern void draw_MI3(uint16_t color, stateType_t *state);
extern void draw_MIMIMI(uint16_t color, stateType_t *state);
extern void draw_IN(uint16_t color, stateType_t *state);
extern void draw_IST2(uint16_t color, stateType_t *state);
extern void draw_AM(uint16_t color, stateType_t *state);
extern void draw_NIE(uint16_t color, stateType_t *state);
extern void draw_GENIE(uint16_t color, stateType_t *state);

#ifdef LAYOUT1
extern void draw_VIEL(uint16_t color, stateType_t *state);
extern void draw_VIELE(uint16_t color, stateType_t *state);
extern void draw_MORGENS(uint16_t color, stateType_t *state);
extern void draw_VORMITTAGS(uint16_t color, stateType_t *state);
extern void draw_ABENDS(uint16_t color, stateType_t *state);
extern void draw_PRAEABENDS(uint16_t color, stateType_t *state);
extern void draw_UHR2(uint16_t color, stateType_t *state);
extern void draw_WENIG(uint16_t color, stateType_t *state);
extern void draw_WENIGE(uint16_t color, stateType_t *state);
#endif

#ifdef LAYOUT2
extern void draw_EIN1(uint16_t color, stateType_t *state);
extern void draw_STEIN(uint16_t color, stateType_t *state);
extern void draw_Std_EIN1(uint16_t color, stateType_t *state);
extern void draw_Std_EIN2(uint16_t color, stateType_t *state);
extern void draw_Std_EINE(uint16_t color, stateType_t *state);
extern void draw_NACHT1(uint16_t color, stateType_t *state);
extern void draw_NACHT2(uint16_t color, stateType_t *state);
extern void draw_TAG1(uint16_t color, stateType_t *state);
extern void draw_TAG2(uint16_t color, stateType_t *state);
extern void draw_Nein1(uint16_t color, stateType_t *state);
extern void draw_ZU(uint16_t color, stateType_t *state);
extern void draw_BIN(uint16_t color, stateType_t *state);
extern void draw_JA(uint16_t color, stateType_t *state);
extern void draw_LAU(uint16_t color, stateType_t *state);
extern void draw_NERV(uint16_t color, stateType_t *state);
extern void draw_BIS(uint16_t color, stateType_t *state);
extern void draw_NICHT(uint16_t color, stateType_t *state);
extern void draw_NICHTS(uint16_t color, stateType_t *state);
extern void draw_DA(uint16_t color, stateType_t *state);
extern void draw_DU(uint16_t color, stateType_t *state);
extern void draw_ZUM(uint16_t color, stateType_t *state);

#endif
#endif