#include <Arduino.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>       //https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_NeoMatrix.h> //https://github.com/adafruit/Adafruit_NeoMatrix
#include <Adafruit_NeoPixel.h>
#include "wordclock.h"


#include "wordclocklayout.h"


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


#define NEOPIXEL_PIN D4


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

static stateType_t State;


uint16_t getColor(){
  uint16_t color = 0;

  switch(State.Color_Mode){
    case COLOR_MODE_MONO:
      color = State.Color;
      break;

     case COLOR_MODE_MIXED:
      color = colors[rand()%MAX_COLORS];
      if(color == State.last_Color){
        color = colors[State.Now.second()%MAX_COLORS];
      }
      break;

    default: 
      color = 0;
  }

  State.last_Color = color;
  return color;
}

void resetGrid(){
  //set all the array values to 0 (off):
  for(int i = 0; i < WIDTH; i++) {
    for(int j = 0; j < HEIGHT; j++) {
      State.last_grid[i][j] = State.grid[i][j];
      State.grid[i][j] = 0;
    }
  }
}



void drawWordClockMin(uint8_t min, bool useAsSeconds){
  uint8_t Min = min;
  if(useAsSeconds == true){
    if(Min == 0){
      draw_Std_NULL(getColor(), &State);
    }
  }
  if(Min != 0){
    if(Min == 1){
      if(useAsSeconds == false){
      draw_Min_EINE(getColor(), &State);
      draw_MINUTE(getColor(), &State);
      draw_NACH(getColor(), &State);
      }else{
        draw_Std_EINS(getColor(), &State);
      }
    }
    if((Min >= 2) && (Min <= 59)){
      if(Min == 2){
        draw_Min_ZWEI(getColor(), &State);
      }
      if(Min == 3){
        draw_Min_DREI(getColor(), &State);
      }
      if(Min == 4){
        draw_Min_VIER(getColor(), &State);
      }
      if(Min == 5){
        draw_Min_FUENF(getColor(), &State);
      }
      if(Min == 6){
        draw_Min_SECHS(getColor(), &State);
      }
      if(Min == 7){
        draw_Min_SIEBEN(getColor(), &State);
      }
      if(Min == 8){
        draw_Min_ACHT(getColor(), &State);
      }
      if(Min == 9){
        draw_Min_NEUN(getColor(), &State);
      }
      if(Min == 10){
        draw_Min_ZEHN(getColor(), &State);
      }
      if(Min == 11){
        draw_Min_ELF(getColor(), &State);
      }
      if(Min == 12){
        draw_Min_ZWOELF(getColor(), &State);
      }
      if(Min == 13){
        draw_Min_DREI(getColor(), &State);
        draw_Min_ZEHN(getColor(), &State);
      }
      if(Min == 14){
        draw_Min_VIER(getColor(), &State);
        draw_Min_ZEHN(getColor(), &State);
      }
      if(Min == 15){
        draw_Min_FUENF(getColor(), &State);
        draw_Min_ZEHN(getColor(), &State);
      }
      if(Min == 16){
        draw_Min_SECHS(getColor(), &State);
        draw_Min_ZEHN(getColor(), &State);
      }
      if(Min == 17){
        draw_Min_SIEB(getColor(), &State);
        draw_Min_ZEHN(getColor(), &State);
      }
      if(Min == 18){
        draw_Min_ACHT(getColor(), &State);
        draw_Min_ZEHN(getColor(), &State);
      }
      if(Min == 19){
        draw_Min_NEUN(getColor(), &State);
        draw_Min_ZEHN(getColor(), &State);
      }
      if(Min == 20){
        draw_Min_ZWANZIG(getColor(), &State);
      }
      if(Min == 21){
        draw_Min_EIN(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_ZWANZIG(getColor(), &State);
      }
      if(Min == 22){
        draw_Min_ZWEI(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_ZWANZIG(getColor(), &State);
      }
      if(Min == 23){
        draw_Min_DREI(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_ZWANZIG(getColor(), &State);
      }
      if(Min == 24){
        draw_Min_VIER(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_ZWANZIG(getColor(), &State);
      }
      if(Min == 25){
        draw_Min_FUENF(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_ZWANZIG(getColor(), &State);
      }
      if(Min == 26){
        draw_Min_SECHS(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_ZWANZIG(getColor(), &State);
      }
      if(Min == 27){
        draw_Min_SIEBEN(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_ZWANZIG(getColor(), &State);
      }
      if(Min == 28){
        draw_Min_ACHT(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_ZWANZIG(getColor(), &State);
      }
      if(Min == 29){
        draw_Min_NEUN(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_ZWANZIG(getColor(), &State);
      }
      if(Min == 30){
        draw_Min_DREISSIG(getColor(), &State);
      }
      if(Min == 31){
        draw_Min_EIN(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_DREISSIG(getColor(), &State);
      }
      if(Min == 32){
        draw_Min_ZWEI(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_DREISSIG(getColor(), &State);
      }
      if(Min == 33){
        draw_Min_DREI(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_DREISSIG(getColor(), &State);
      }
      if(Min == 34){
        draw_Min_VIER(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_DREISSIG(getColor(), &State);
      }
      if(Min == 35){
        draw_Min_FUENF(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_DREISSIG(getColor(), &State);
      }
      if(Min == 36){
        draw_Min_SECHS(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_DREISSIG(getColor(), &State);
      }
      if(Min == 37){
        draw_Min_SIEBEN(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_DREISSIG(getColor(), &State);
      }
      if(Min == 38){
        draw_Min_ACHT(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_DREISSIG(getColor(), &State);
      }
      if(Min == 39){
        draw_Min_NEUN(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_DREISSIG(getColor(), &State);
      }
      if(Min == 40){
        draw_Min_VIERZIG(getColor(), &State);
      }
      if(Min == 41){
        draw_Min_EIN(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_VIERZIG(getColor(), &State);
      }
      if(Min == 42){
        draw_Min_ZWEI(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_VIERZIG(getColor(), &State);
      }
      if(Min == 43){
        draw_Min_DREI(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_VIERZIG(getColor(), &State);
      }
      if(Min == 44){
        draw_Min_VIER(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_VIERZIG(getColor(), &State);
      }
      if(Min == 45){
        draw_Min_FUENF(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_VIERZIG(getColor(), &State);
      }
      if(Min == 46){
        draw_Min_SECHS(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_VIERZIG(getColor(), &State);
      }
      if(Min == 47){
        draw_Min_SIEBEN(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_VIERZIG(getColor(), &State);
      }
      if(Min == 48){
        draw_Min_ACHT(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_VIERZIG(getColor(), &State);
      }
      if(Min == 49){
        draw_Min_NEUN(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_VIERZIG(getColor(), &State);
      }
      if(Min == 50){
        draw_Min_FUENFZIG(getColor(), &State);
      }
      if(Min == 51){
        draw_Min_EIN(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_FUENFZIG(getColor(), &State);
      }
      if(Min == 52){
        draw_Min_ZWEI(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_FUENFZIG(getColor(), &State);
      }
      if(Min == 53){
        draw_Min_DREI(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_FUENFZIG(getColor(), &State);
      }
      if(Min == 54){
        draw_Min_VIER(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_FUENFZIG(getColor(), &State);
      }
      if(Min == 55){
        draw_Min_FUENF(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_FUENFZIG(getColor(), &State);
      }
      if(Min == 56){
        draw_Min_SECHS(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_FUENFZIG(getColor(), &State);
      }
      if(Min == 57){
        draw_Min_SIEBEN(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_FUENFZIG(getColor(), &State);
      }
      if(Min == 58){
        draw_Min_ACHT(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_FUENFZIG(getColor(), &State);
      }
      if(Min == 59){
        draw_Min_NEUN(getColor(), &State);
        draw_Min_UND(getColor(), &State);
        draw_Min_FUENFZIG(getColor(), &State);
      }
      if(useAsSeconds == false){
        draw_MINUTEN(getColor(), &State);
        draw_NACH(getColor(), &State);
      }
    }
  }
}

void drawWordClockHour(){
  uint8_t h = State.Now.hour();
  uint8_t Min = State.Now.minute();
  if(h == 0){
    draw_Std_NULL(getColor(), &State);
  }
  if(h == 1){
    if(Min != 0){
      draw_Std_EINS(getColor(), &State);
    }else{
      draw_Std_EIN(getColor(), &State);
    }
  }
  if(h == 2){
    draw_Std_ZWEI(getColor(), &State);
  }
  if(h == 3){
    draw_Std_DREI(getColor(), &State);
  }
  if(h == 4){
    draw_Std_VIER(getColor(), &State);
  }
  if(h == 5){
    draw_Std_FUENF(getColor(), &State);
  }
  if(h == 6){
    draw_Std_SECHS(getColor(), &State);
  }
  if(h == 7){
    draw_Std_SIEBEN(getColor(), &State);
  }
  if(h == 8){
    draw_Std_ACHT(getColor(), &State);
  }
  if(h == 9){
    draw_Std_NEUN(getColor(), &State);
  }
  if(h == 10){
    draw_Std_ZEHN(getColor(), &State);
  }
  if(h == 11){
    draw_Std_ELF(getColor(), &State);
  }
  if(h == 12){
    draw_Std_ZWOELF(getColor(), &State);
  }

  //todo alternate UHR1 and UHR2
  draw_UHR1(getColor(), &State);

}

void draw_daytime(){
  draw_ES_IST(getColor(), &State);
  if(State.Now.hour()<= 4){
    draw_SPAET(getColor(), &State);
    draw_NACHTS(getColor(), &State);
  }
  if((State.Now.hour()>4)&&(State.Now.hour()<= 9)){
    draw_MORGEN(getColor(), &State);
  }
  if((State.Now.hour()>9)&&(State.Now.hour()<= 11)){
    draw_VORMITTAG(getColor(), &State);
  }
  if((State.Now.hour()>11)&&(State.Now.hour()<= 14)){
    draw_MITTAGS(getColor(), &State);
  }
  if((State.Now.hour()>14)&&(State.Now.hour()<= 16)){
    draw_NACH(getColor(), &State);
    draw_MITTAG(getColor(), &State);
  }
  if((State.Now.hour()>16)&&(State.Now.hour()<= 19)){
    draw_PRAEABEND(getColor(), &State);
  }
  if((State.Now.hour()>19)&&(State.Now.hour()<= 22)){
    draw_ABEND(getColor(), &State);
  }
  if((State.Now.hour()>22)){
    draw_NACHT(getColor(), &State);
  }
}

//draw a 3x5 Zero, x,y is upper left corner
void draw_Binary_Zero(uint8_t x, uint8_t y, uint16_t color){
  if((x<22)&&(y<8)){
    State.grid[x][y] = color;
    State.grid[x+1][y] = color;
    State.grid[x+2][y] = color;
    State.grid[x][y+1] = color;
    State.grid[x][y+2] = color;
    State.grid[x][y+3] = color;
    State.grid[x][y+4] = color;
    State.grid[x+2][y+1] = color;
    State.grid[x+2][y+2] = color;
    State.grid[x+2][y+3] = color;
    State.grid[x+2][y+4] = color;
    State.grid[x][y+1] = color;
    State.grid[x][y+2] = color;
    State.grid[x][y+3] = color;
    State.grid[x+1][y+4] = color;
  }
}

//draw a 3x5 One, x,y is upper left corner
void draw_Binary_One(uint8_t x, uint8_t y, uint16_t color){
  if((x<22)&&(y<8)){
    State.grid[x+1][y] = color;
    State.grid[x+1][y+1] = color;
    State.grid[x+1][y+2] = color;
    State.grid[x+1][y+3] = color;
    State.grid[x+1][y+4] = color;
    State.grid[x+2][y+4] = color;
    State.grid[x][y+4] = color;
    State.grid[x][y+1] = color;
  }
}

void draw_Binary_Time(uint16_t color){
  draw_ES_IST(color, &State);

  if((State.Now.hour() & (1<<0))){
    draw_Binary_One(21, 0, color);
  }else{
    draw_Binary_Zero(21, 0, color);
  }
  
  if((State.Now.hour() & (1<<1))){
    draw_Binary_One(17, 0, color);
  }else{
    draw_Binary_Zero(17, 0, color);
  }
  
  if((State.Now.hour() & (1<<2))){
    draw_Binary_One(13, 0, color);
  }else{
    draw_Binary_Zero(13, 0, color);
  }
  
  if((State.Now.hour() & (1<<3))){
    draw_Binary_One(9, 0, color);
  }else{
    draw_Binary_Zero(9, 0, color);
  }

  
  if((State.Now.minute() & (1<<0))){
    draw_Binary_One(21, 7, color);
  }else{
    draw_Binary_Zero(21, 7, color);
  }
  
  if((State.Now.minute() & (1<<1))){
    draw_Binary_One(17, 7, color);
  }else{
    draw_Binary_Zero(17, 7, color);
  }
  
  if((State.Now.minute() & (1<<2))){
    draw_Binary_One(13, 7, color);
  }else{
    draw_Binary_Zero(13, 7, color);
  }
  
  if((State.Now.minute() & (1<<3))){
    draw_Binary_One(9, 7, color);
  }else{
    draw_Binary_Zero(9, 7, color);
  }
  
  if((State.Now.minute() & (1<<4))){
    draw_Binary_One(5, 7, color);
  }else{
    draw_Binary_Zero(5, 7, color);
  }
  
  if((State.Now.minute() & (1<<5))){
    draw_Binary_One(1, 7, color);
  }else{
    draw_Binary_Zero(1, 7, color);
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
  matrix.setBrightness(255);
  matrix.setTextColor(colors[1]);

  State.Mode = MODE_WORD_CLOCK;
  //State.Mode = MODE_WORD_CLOCK_SECONDS; //MODE_BINARY_CLOCK; //MODE_MARQUEE_TIME;
  State.last_Color = colors[1];
  State.Color_Mode = COLOR_MODE_MONO;
  State.Color = colors[5];

  Serial.println("Wordclock");
  Serial.print(State.Now.minute());
  Serial.println(" Min");
  Serial.print(State.Now.hour());
  Serial.println(" hour");
}



int x    = matrix.width();
int pass = 0;
uint8_t CountDownSeconds = 0;

// the loop function runs over and over again forever
void loop() {
  State.Now = RTC.now();
  resetGrid();

  if(State.Mode == MODE_WORD_CLOCK){
    draw_ES_IST(getColor(), &State);
    drawWordClockMin(State.Now.minute(),false);
    drawWordClockHour();
  }

  if(State.Mode == MODE_WORD_CLOCK_SECONDS){
    drawWordClockMin(State.Now.second(),true);
  }

  if(State.Mode == MODE_WORD_CLOCK_COUNT_DOWN){
    if(State.Now.second() !=0){
      CountDownSeconds = 60 - State.Now.second();
    }else{
      CountDownSeconds = 0;
    }
    drawWordClockMin(CountDownSeconds,true);
  }

  if(State.Mode == MODE_BINARY_CLOCK){
    draw_Binary_Time(getColor());
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
    //matrix.print(F("Leon!"));
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
