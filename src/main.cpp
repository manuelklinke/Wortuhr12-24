#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>       //https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_NeoMatrix.h> //https://github.com/adafruit/Adafruit_NeoMatrix
#include <Adafruit_NeoPixel.h>
#include "wordclock.h"
#include "secrets.h"


#include "wordclocklayout.h"

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
#define MAX_FACTS 14

const uint16_t SETTINGS_MAGIC = 0x1224;
const uint8_t SETTINGS_VERSION = 1;

typedef struct
{
  uint16_t magic;
  uint8_t version;
  uint8_t mode;
  uint8_t wordClockMode;
  uint8_t colorMode;
  uint8_t rainbowMode;
  uint16_t color;
  uint8_t brightness;
  char marqueeText[MAX_MARQUEE_TEXT_LENGTH];
} persistedSettings_t;

const size_t EEPROM_SIZE = sizeof(persistedSettings_t);

RTC_DS1307 RTC; // Establish clock object
ESP8266WebServer webServer(80);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(WIDTH, HEIGHT, NEOPIXEL_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS    + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);


// Resistor color code digits 0-9.
const uint16_t colors[] = {
  matrix.Color(0, 0, 0),        // 0 black
  matrix.Color(120, 60, 0),     // 1 brown
  matrix.Color(255, 0, 0),      // 2 red
  matrix.Color(255, 80, 0),     // 3 orange
  matrix.Color(255, 255, 0),    // 4 yellow
  matrix.Color(0, 255, 0),      // 5 green
  matrix.Color(0, 0, 255),      // 6 blue
  matrix.Color(128, 0, 255),    // 7 violet
  matrix.Color(128, 128, 128),  // 8 grey
  matrix.Color(255, 255, 255) };// 9 white

DateTime Now;

static stateType_t State;

int x = 0;
int pass = 0;
uint8_t CountDownSeconds = 0;
bool ForceDisplayUpdate = true;
unsigned long LastWifiReconnectAttempt = 0;
unsigned long LastMqttReconnectAttempt = 0;

bool isValidMode(uint8_t mode){
  switch(mode){
    case MODE_WORD_CLOCK:
    case MODE_WORD_CLOCK_SECONDS:
    case MODE_WORD_CLOCK_COUNT_DOWN:
    case MODE_WORD_CLOCK_DAYTIME:
    case MODE_MARQUEE_TIME:
    case MODE_BINARY_CLOCK:
    case MODE_TIME_FOR_FACTS:
    case MODE_MARQUEE_TEXT:
      return true;
    default:
      return false;
  }
}

bool isValidWordClockMode(uint8_t mode){
  return mode <= WORD_CLOCK_MODE_COUNTDOWN_SECOND_HALF_MINUTES;
}

bool isValidColorMode(uint8_t mode){
  return mode <= COLOR_MODE_RAINBOW;
}

bool isValidRainbowMode(uint8_t mode){
  return mode <= RAINBOW_MODE_WORD_STARTS;
}

void setDefaultSettings(){
  State.Mode = MODE_WORD_CLOCK;
  State.Word_Clock_Mode = WORD_CLOCK_MODE_LONG;
  State.last_Color = colors[1];
  State.Color_Mode = COLOR_MODE_MONO;
  State.Rainbow_Mode = RAINBOW_MODE_MATRIX;
  State.Color = colors[5];
  State.Brightness = BRIGHTNESS_DAY;
  strlcpy(State.Marquee_Text, "Hallo von der Wortuhr", sizeof(State.Marquee_Text));
}

void saveSettings(){
  persistedSettings_t settings;
  settings.magic = SETTINGS_MAGIC;
  settings.version = SETTINGS_VERSION;
  settings.mode = State.Mode;
  settings.wordClockMode = State.Word_Clock_Mode;
  settings.colorMode = State.Color_Mode;
  settings.rainbowMode = State.Rainbow_Mode;
  settings.color = State.Color;
  settings.brightness = State.Brightness;
  strlcpy(settings.marqueeText, State.Marquee_Text, sizeof(settings.marqueeText));

  EEPROM.put(0, settings);
  EEPROM.commit();
}

bool loadSettings(){
  persistedSettings_t settings;
  EEPROM.get(0, settings);

  if(settings.magic != SETTINGS_MAGIC || settings.version != SETTINGS_VERSION){
    return false;
  }
  if(!isValidMode(settings.mode) ||
     !isValidWordClockMode(settings.wordClockMode) ||
     !isValidColorMode(settings.colorMode) ||
     !isValidRainbowMode(settings.rainbowMode) ||
     settings.brightness == 0){
    return false;
  }

  State.Mode = settings.mode;
  State.Word_Clock_Mode = settings.wordClockMode;
  State.Color_Mode = settings.colorMode;
  State.Rainbow_Mode = settings.rainbowMode;
  State.Color = settings.color;
  State.Brightness = settings.brightness;
  settings.marqueeText[sizeof(settings.marqueeText) - 1] = '\0';
  strlcpy(State.Marquee_Text, settings.marqueeText, sizeof(State.Marquee_Text));
  State.last_Color = State.Color;
  return true;
}

uint16_t colorWheel(byte pos){
  pos = 255 - pos;
  if(pos < 85) {
    return matrix.Color(255 - pos * 3, 0, pos * 3);
  }
  if(pos < 170) {
    pos -= 85;
    return matrix.Color(0, pos * 3, 255 - pos * 3);
  }
  pos -= 170;
  return matrix.Color(pos * 3, 255 - pos * 3, 0);
}

uint16_t colorFromHex(const String &hexColor){
  String value = hexColor;
  value.trim();
  if(value.startsWith("#")){
    value.remove(0, 1);
  }
  if(value.length() != 6){
    return State.Color;
  }

  uint32_t rgb = strtoul(value.c_str(), nullptr, 16);
  return matrix.Color((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, rgb & 0xff);
}

String colorToHex(uint16_t color){
  uint8_t r = ((color >> 11) & 0x1f) * 255 / 31;
  uint8_t g = ((color >> 5) & 0x3f) * 255 / 63;
  uint8_t b = (color & 0x1f) * 255 / 31;
  char buffer[8];
  snprintf(buffer, sizeof(buffer), "#%02X%02X%02X", r, g, b);
  return String(buffer);
}

const char* modeToName(int mode){
  switch(mode){
    case MODE_WORD_CLOCK: return "word_clock";
    case MODE_WORD_CLOCK_SECONDS: return "seconds";
    case MODE_WORD_CLOCK_COUNT_DOWN: return "countdown";
    case MODE_WORD_CLOCK_DAYTIME: return "daytime";
    case MODE_MARQUEE_TIME: return "marquee_time";
    case MODE_BINARY_CLOCK: return "binary";
    case MODE_TIME_FOR_FACTS: return "facts";
    case MODE_MARQUEE_TEXT: return "marquee_text";
    default: return "unknown";
  }
}

int modeFromName(const String &name){
  if(name == "word_clock") return MODE_WORD_CLOCK;
  if(name == "seconds") return MODE_WORD_CLOCK_SECONDS;
  if(name == "countdown") return MODE_WORD_CLOCK_COUNT_DOWN;
  if(name == "daytime") return MODE_WORD_CLOCK_DAYTIME;
  if(name == "marquee_time") return MODE_MARQUEE_TIME;
  if(name == "binary") return MODE_BINARY_CLOCK;
  if(name == "facts") return MODE_TIME_FOR_FACTS;
  if(name == "marquee_text") return MODE_MARQUEE_TEXT;
  return State.Mode;
}

const char* colorModeToName(){
  if(State.Color_Mode == COLOR_MODE_MONO) return "mono";
  if(State.Color_Mode == COLOR_MODE_MIXED) return "mixed";
  if(State.Color_Mode == COLOR_MODE_RAINBOW){
    if(State.Rainbow_Mode == RAINBOW_MODE_SIDE_GRADIENT) return "rainbow_side";
    if(State.Rainbow_Mode == RAINBOW_MODE_WORD_STARTS) return "rainbow_words";
    return "rainbow_matrix";
  }
  return "unknown";
}

void setColorModeFromName(const String &name){
  if(name == "mono"){
    State.Color_Mode = COLOR_MODE_MONO;
  }else if(name == "mixed"){
    State.Color_Mode = COLOR_MODE_MIXED;
  }else if(name == "rainbow_side"){
    State.Color_Mode = COLOR_MODE_RAINBOW;
    State.Rainbow_Mode = RAINBOW_MODE_SIDE_GRADIENT;
  }else if(name == "rainbow_words"){
    State.Color_Mode = COLOR_MODE_RAINBOW;
    State.Rainbow_Mode = RAINBOW_MODE_WORD_STARTS;
  }else if(name == "rainbow_matrix"){
    State.Color_Mode = COLOR_MODE_RAINBOW;
    State.Rainbow_Mode = RAINBOW_MODE_MATRIX;
  }
}

bool parseDateTime(const String &value, DateTime *dateTime){
  int year, month, day, hour, minute, second;
  if(sscanf(value.c_str(), "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &minute, &second) == 6){
    *dateTime = DateTime(year, month, day, hour, minute, second);
    return true;
  }
  if(sscanf(value.c_str(), "%d-%d-%dT%d:%d", &year, &month, &day, &hour, &minute) == 5){
    *dateTime = DateTime(year, month, day, hour, minute, 0);
    return true;
  }
  return false;
}

String currentDateTimeValue(){
  char buffer[25];
  snprintf(buffer, sizeof(buffer), "%04d-%02d-%02dT%02d:%02d:%02d",
           Now.year(), Now.month(), Now.day(), Now.hour(), Now.minute(), Now.second());
  return String(buffer);
}

uint16_t getColor(){
  uint16_t color = 0;

  switch(State.Color_Mode){
    case COLOR_MODE_MONO:
      color = State.Color;
      break;

     case COLOR_MODE_MIXED:
      color = colors[rand()%MAX_COLORS];
      if(color == State.last_Color){
        color = colors[Now.second()%MAX_COLORS];
      }
      break;

    case COLOR_MODE_RAINBOW:
      color = colorWheel((millis() / 20 + State.Color_Counter * 17) & 0xff);
      State.Color_Counter++;
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
      }
    }
  }
}

void drawWordClockHour(){
  uint8_t h = Now.twelveHour();
  if(h == 0){
    draw_Std_NULL(getColor(), &State);
  }
  if(h == 1){
    
    draw_Std_EIN(getColor(), &State);
    
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
  if(Now.hour()<= 4){
    draw_SPAET(getColor(), &State);
#ifdef LAYOUT2
    draw_NACHT1(getColor(), &State);
#else
    draw_NACHTS(getColor(), &State);
#endif
  }
  if((Now.hour()>4)&&(Now.hour()<= 9)){
    draw_MORGEN(getColor(), &State);
  }
  if((Now.hour()>9)&&(Now.hour()<= 11)){
    draw_VORMITTAG(getColor(), &State);
  }
  if((Now.hour()>11)&&(Now.hour()<= 14)){
#ifdef LAYOUT2
    draw_MITTAG(getColor(), &State);
#else
    draw_MITTAGS(getColor(), &State);
#endif
  }
  if((Now.hour()>14)&&(Now.hour()<= 16)){
    draw_NACH(getColor(), &State);
    draw_MITTAG(getColor(), &State);
  }
  if((Now.hour()>16)&&(Now.hour()<= 19)){
    draw_PRAEABEND(getColor(), &State);
  }
  if((Now.hour()>19)&&(Now.hour()<= 22)){
    draw_ABEND(getColor(), &State);
  }
  if((Now.hour()>22)){
#ifdef LAYOUT2
    draw_NACHT1(getColor(), &State);
#else
    draw_NACHT(getColor(), &State);
#endif
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
  resetGrid();
  draw_ES_IST(color, &State);

  if((Now.hour() & (1<<0))){
    draw_Binary_One(21, 0, color);
  }else{
    draw_Binary_Zero(21, 0, color);
  }
  
  if((Now.hour() & (1<<1))){
    draw_Binary_One(17, 0, color);
  }else{
    draw_Binary_Zero(17, 0, color);
  }
  
  if((Now.hour() & (1<<2))){
    draw_Binary_One(13, 0, color);
  }else{
    draw_Binary_Zero(13, 0, color);
  }
  
  if((Now.hour() & (1<<3))){
    draw_Binary_One(9, 0, color);
  }else{
    draw_Binary_Zero(9, 0, color);
  }

  
  if((Now.minute() & (1<<0))){
    draw_Binary_One(21, 7, color);
  }else{
    draw_Binary_Zero(21, 7, color);
  }
  
  if((Now.minute() & (1<<1))){
    draw_Binary_One(17, 7, color);
  }else{
    draw_Binary_Zero(17, 7, color);
  }
  
  if((Now.minute() & (1<<2))){
    draw_Binary_One(13, 7, color);
  }else{
    draw_Binary_Zero(13, 7, color);
  }
  
  if((Now.minute() & (1<<3))){
    draw_Binary_One(9, 7, color);
  }else{
    draw_Binary_Zero(9, 7, color);
  }
  
  if((Now.minute() & (1<<4))){
    draw_Binary_One(5, 7, color);
  }else{
    draw_Binary_Zero(5, 7, color);
  }
  
  if((Now.minute() & (1<<5))){
    draw_Binary_One(1, 7, color);
  }else{
    draw_Binary_Zero(1, 7, color);
  }
}

void applyDisplaySettings(){
  matrix.setBrightness(State.Brightness);
  matrix.setTextColor(State.Color);
  ForceDisplayUpdate = true;
  x = matrix.width();
}

String selectedAttr(const char *value, const char *current){
  return strcmp(value, current) == 0 ? " selected" : "";
}

String buildStateJson(){
  String json = "{";
  json += "\"mode\":\"";
  json += modeToName(State.Mode);
  json += "\",\"color_mode\":\"";
  json += colorModeToName();
  json += "\",\"color\":\"";
  json += colorToHex(State.Color);
  json += "\",\"brightness\":";
  json += String(State.Brightness);
  json += ",\"text\":\"";
  json += State.Marquee_Text;
  json += "\",\"time\":\"";
  json += currentDateTimeValue();
  json += "\",\"ip\":\"";
  json += WiFi.localIP().toString();
  json += "\"}";
  return json;
}

String buildSettingsPage(){
  const char *modeName = modeToName(State.Mode);
  const char *colorModeName = colorModeToName();
  String html;
  html.reserve(5200);
  html += F("<!doctype html><html lang=\"de\"><head><meta charset=\"utf-8\">");
  html += F("<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">");
  html += F("<title>Wortuhr</title><style>");
  html += F("body{font-family:system-ui,-apple-system,Segoe UI,sans-serif;margin:0;background:#101318;color:#f5f7fb}");
  html += F("main{max-width:760px;margin:0 auto;padding:24px}h1{font-size:1.6rem;margin:0 0 20px}");
  html += F("form{display:grid;gap:16px}label{display:grid;gap:6px;font-weight:600}");
  html += F("select,input,textarea,button{font:inherit;border:1px solid #394150;border-radius:6px;padding:10px;background:#171c24;color:#f5f7fb}");
  html += F("input[type=color]{height:44px;padding:4px}input[type=range]{padding:0}");
  html += F(".row{display:grid;grid-template-columns:1fr 1fr;gap:16px}.status{color:#aab3c2;margin-bottom:18px}");
  html += F("button{background:#2c7be5;border-color:#2c7be5;font-weight:700;cursor:pointer}");
  html += F("@media(max-width:640px){.row{grid-template-columns:1fr}main{padding:18px}}");
  html += F("</style></head><body><main><h1>Wortuhr Einstellungen</h1>");
  html += F("<p class=\"status\">IP: ");
  html += WiFi.localIP().toString();
  html += F(" | MQTT: ");
  html += mqttClient.connected() ? F("verbunden") : F("nicht verbunden");
  html += F("</p><form method=\"post\" action=\"/settings\"><div class=\"row\"><label>Modus<select name=\"mode\">");
  html += F("<option value=\"word_clock\"");
  html += selectedAttr("word_clock", modeName);
  html += F(">Wortuhr</option><option value=\"seconds\"");
  html += selectedAttr("seconds", modeName);
  html += F(">Sekunden</option><option value=\"countdown\"");
  html += selectedAttr("countdown", modeName);
  html += F(">Countdown</option><option value=\"daytime\"");
  html += selectedAttr("daytime", modeName);
  html += F(">Tageszeit</option><option value=\"marquee_time\"");
  html += selectedAttr("marquee_time", modeName);
  html += F(">Lauftext Uhrzeit</option><option value=\"marquee_text\"");
  html += selectedAttr("marquee_text", modeName);
  html += F(">Freier Lauftext</option><option value=\"binary\"");
  html += selectedAttr("binary", modeName);
  html += F(">Binaeruhr</option><option value=\"facts\"");
  html += selectedAttr("facts", modeName);
  html += F(">Sprueche</option></select></label>");
  html += F("<label>Farbmodus<select name=\"color_mode\"><option value=\"mono\"");
  html += selectedAttr("mono", colorModeName);
  html += F(">Einfarbig</option><option value=\"mixed\"");
  html += selectedAttr("mixed", colorModeName);
  html += F(">Gemischt</option><option value=\"rainbow_matrix\"");
  html += selectedAttr("rainbow_matrix", colorModeName);
  html += F(">Rainbow Matrix</option><option value=\"rainbow_side\"");
  html += selectedAttr("rainbow_side", colorModeName);
  html += F(">Rainbow Seite</option><option value=\"rainbow_words\"");
  html += selectedAttr("rainbow_words", colorModeName);
  html += F(">Rainbow Woerter</option></select></label></div>");
  html += F("<div class=\"row\"><label>Farbe<input type=\"color\" name=\"color\" value=\"");
  html += colorToHex(State.Color);
  html += F("\"></label><label>Helligkeit<input type=\"range\" min=\"1\" max=\"255\" name=\"brightness\" value=\"");
  html += String(State.Brightness);
  html += F("\"></label></div>");
  html += F("<label>Uhrzeit setzen<input type=\"datetime-local\" step=\"1\" name=\"time\" value=\"");
  html += currentDateTimeValue();
  html += F("\"></label><label>Freier Lauftext<textarea name=\"text\" maxlength=\"95\" rows=\"3\">");
  html += State.Marquee_Text;
  html += F("</textarea></label><button type=\"submit\">Uebernehmen und speichern</button></form></main></body></html>");
  return html;
}

void publishState(){
  if(!mqttClient.connected()){
    return;
  }
  char topic[96];
  snprintf(topic, sizeof(topic), "%s/mode", MQTT_TOPIC_BASE);
  mqttClient.publish(topic, modeToName(State.Mode), true);
  snprintf(topic, sizeof(topic), "%s/color_mode", MQTT_TOPIC_BASE);
  mqttClient.publish(topic, colorModeToName(), true);
  snprintf(topic, sizeof(topic), "%s/color", MQTT_TOPIC_BASE);
  String hexColor = colorToHex(State.Color);
  mqttClient.publish(topic, hexColor.c_str(), true);
  snprintf(topic, sizeof(topic), "%s/brightness", MQTT_TOPIC_BASE);
  char brightness[4];
  snprintf(brightness, sizeof(brightness), "%u", State.Brightness);
  mqttClient.publish(topic, brightness, true);
  snprintf(topic, sizeof(topic), "%s/text", MQTT_TOPIC_BASE);
  mqttClient.publish(topic, State.Marquee_Text, true);
  snprintf(topic, sizeof(topic), "%s/ip", MQTT_TOPIC_BASE);
  mqttClient.publish(topic, WiFi.localIP().toString().c_str(), true);
}

void handleSettingsPost(){
  if(webServer.hasArg("mode")){
    State.Mode = modeFromName(webServer.arg("mode"));
  }
  if(webServer.hasArg("color_mode")){
    setColorModeFromName(webServer.arg("color_mode"));
  }
  if(webServer.hasArg("color")){
    State.Color = colorFromHex(webServer.arg("color"));
  }
  if(webServer.hasArg("brightness")){
    State.Brightness = constrain(webServer.arg("brightness").toInt(), 1, 255);
  }
  if(webServer.hasArg("text")){
    webServer.arg("text").toCharArray(State.Marquee_Text, sizeof(State.Marquee_Text));
  }
  if(webServer.hasArg("time") && webServer.arg("time").length() > 0){
    DateTime newTime;
    if(parseDateTime(webServer.arg("time"), &newTime)){
      RTC.adjust(newTime);
      Now = RTC.now();
    }
  }

  applyDisplaySettings();
  saveSettings();
  publishState();
  webServer.sendHeader("Location", "/");
  webServer.send(303, "text/plain", "");
}

void setupWebServer(){
  webServer.on("/", HTTP_GET, [](){
    webServer.send(200, "text/html", buildSettingsPage());
  });
  webServer.on("/settings", HTTP_POST, handleSettingsPost);
  webServer.on("/api/state", HTTP_GET, [](){
    webServer.send(200, "application/json", buildStateJson());
  });
  webServer.on("/api/save", HTTP_POST, [](){
    saveSettings();
    publishState();
    webServer.send(200, "text/plain", "saved");
  });
  webServer.begin();
}

void handleMqttMessage(char* topic, byte* payload, unsigned int length){
  char message[128];
  unsigned int copyLength = min(length, sizeof(message) - 1);
  memcpy(message, payload, copyLength);
  message[copyLength] = '\0';
  String value = String(message);
  value.trim();
  String topicName = String(topic);
  bool shouldSave = false;

  if(topicName.endsWith("/mode/set")){
    State.Mode = modeFromName(value);
    shouldSave = true;
  }else if(topicName.endsWith("/color_mode/set")){
    setColorModeFromName(value);
    shouldSave = true;
  }else if(topicName.endsWith("/color/set")){
    State.Color = colorFromHex(value);
    shouldSave = true;
  }else if(topicName.endsWith("/brightness/set")){
    State.Brightness = constrain(value.toInt(), 1, 255);
    shouldSave = true;
  }else if(topicName.endsWith("/text/set")){
    value.toCharArray(State.Marquee_Text, sizeof(State.Marquee_Text));
    State.Mode = MODE_MARQUEE_TEXT;
    shouldSave = true;
  }else if(topicName.endsWith("/time/set")){
    DateTime newTime;
    if(parseDateTime(value, &newTime)){
      RTC.adjust(newTime);
      Now = RTC.now();
    }
  }else if(topicName.endsWith("/save") && value == "1"){
    shouldSave = true;
  }else if(topicName.endsWith("/reboot") && value == "1"){
    ESP.restart();
  }

  applyDisplaySettings();
  if(shouldSave){
    saveSettings();
  }
  publishState();
}

void subscribeMqttTopics(){
  char topic[96];
  snprintf(topic, sizeof(topic), "%s/+/set", MQTT_TOPIC_BASE);
  mqttClient.subscribe(topic);
  snprintf(topic, sizeof(topic), "%s/reboot", MQTT_TOPIC_BASE);
  mqttClient.subscribe(topic);
  snprintf(topic, sizeof(topic), "%s/save", MQTT_TOPIC_BASE);
  mqttClient.subscribe(topic);
}

void connectMqtt(){
  if(strlen(MQTT_HOST) == 0 || WiFi.status() != WL_CONNECTED || mqttClient.connected()){
    return;
  }

  char willTopic[96];
  snprintf(willTopic, sizeof(willTopic), "%s/status", MQTT_TOPIC_BASE);
  if(mqttClient.connect(MQTT_CLIENT_ID, willTopic, 1, true, "offline")){
    mqttClient.publish(willTopic, "online", true);
    subscribeMqttTopics();
    publishState();
  }
}

void handleNetwork(){
  webServer.handleClient();
  if(WiFi.status() != WL_CONNECTED && millis() - LastWifiReconnectAttempt > 10000){
    LastWifiReconnectAttempt = millis();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }

  if(strlen(MQTT_HOST) > 0){
    if(!mqttClient.connected() && millis() - LastMqttReconnectAttempt > 5000){
      LastMqttReconnectAttempt = millis();
      connectMqtt();
    }
    mqttClient.loop();
  }
}

/// the setup function runs once when you press reset or power the board
void setup() {
  x    = matrix.width();
  Serial.begin(115200);
  Wire.begin();  // Begin I2C
  RTC.begin();   // begin clock
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(255);
  matrix.setTextColor(colors[1]);

  EEPROM.begin(EEPROM_SIZE);
  setDefaultSettings();
  if(!loadSettings()){
    saveSettings();
  }
  applyDisplaySettings();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  setupWebServer();
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(handleMqttMessage);

  
}




// the loop function runs over and over again forever
void loop() {
  static uint8_t ActualMin = 60;
  static uint8_t ActualSec = 60;

  handleNetwork();
  Now = RTC.now();

  if(ForceDisplayUpdate){
    ActualMin = 60;
    ActualSec = 60;
    resetGrid();
    ForceDisplayUpdate = false;
  }
  

  if(State.Mode == MODE_WORD_CLOCK){
    if(ActualMin != Now.minute()){
      resetGrid();
      draw_ES_IST(getColor(), &State);
      drawWordClockHour();
      draw_Std_UND(getColor(), &State);
      drawWordClockMin(Now.minute(),false);
      
      ActualMin = Now.minute();
    }
    
  }

  if(State.Mode == MODE_WORD_CLOCK_DAYTIME){
    if(ActualMin != Now.minute()){
      resetGrid();
      draw_daytime();
      ActualMin = Now.minute();
    }
  }

  if(State.Mode == MODE_WORD_CLOCK_SECONDS){
    if(ActualSec != Now.second()){
      resetGrid();
      drawWordClockMin(Now.second(),true);
      ActualSec = Now.second();
    }
  }

  if(State.Mode == MODE_WORD_CLOCK_COUNT_DOWN){
    if(ActualSec != Now.second()){
      resetGrid();
      if(Now.second() !=0){
        CountDownSeconds = 60 - Now.second();
      }else{
        CountDownSeconds = 0;
      }
      drawWordClockMin(CountDownSeconds,true);
      ActualSec = Now.second();
    }
  }

  if(State.Mode == MODE_BINARY_CLOCK){
    if(ActualSec != Now.second()){
      draw_Binary_Time(getColor());
      ActualSec = Now.second();
    }
  }

  if(State.Mode == MODE_TIME_FOR_FACTS){
      
      
      if(ActualMin != Now.minute()){
        resetGrid();
      switch (random(1, MAX_FACTS + 1))
      {
      case 1:
        draw_ES_IST(getColor(), &State);
        draw_ZEIT(getColor(),&State);
        draw_FUER(getColor(), &State);
        draw_BIER(getColor(), &State);
        break;
      case 2:
        draw_ES_IST(getColor(), &State);
        draw_ZEIT(getColor(),&State);
        draw_FUER(getColor(), &State);
        draw_URLAUB(getColor(), &State);
        break;
      case 3:
        draw_ES_IST(getColor(), &State);
        draw_ZEIT(getColor(),&State);
        draw_FUER(getColor(), &State);
        draw_MIMIMI(getColor(), &State);
        break;
      case 4:
        draw_ES_IST(getColor(), &State);
        draw_ZEIT(getColor(),&State);
        draw_FUER(getColor(), &State);
        draw_WEIN(getColor(), &State);
        break;
      case 5:
        draw_ES_IST(getColor(), &State);
        draw_ZEIT(getColor(),&State);
        draw_FUER(getColor(), &State);
        draw_EIS(getColor(), &State);
        break;
      case 6:
        draw_ES_IST(getColor(), &State);
        draw_ZEIT(getColor(),&State);
        draw_FUER(getColor(), &State);
        draw_KAFFEE(getColor(), &State);
        break;
      case 7:
        draw_NERV(getColor(), &State);
        draw_NICHT(getColor(), &State);
        break;
      case 8:
        draw_ES_IST(getColor(), &State);
        draw_ZEIT(getColor(),&State);
        draw_FUER(getColor(), &State);
        draw_NIX(getColor(), &State);
        break;
      case 9:
        draw_ES_IST(getColor(), &State);
        draw_ZEIT(getColor(),&State);
        draw_FUER(getColor(), &State);
        draw_PRAEABEND(getColor(), &State);
        break;
      case 10:
        draw_ES_IST(getColor(), &State);
        draw_NOCH(getColor(), &State);
        draw_KAFFEE(getColor(), &State);
        draw_DA(getColor(), &State);
        break;
      case 11:
        draw_ES_IST(getColor(), &State);
        draw_NOCH(getColor(), &State);
        draw_BIER(getColor(), &State);
        draw_DA(getColor(), &State);
        break;
      case 12:
        draw_ES_IST(getColor(), &State);
        draw_NOCH(getColor(), &State);
        draw_WEIN(getColor(), &State);
        draw_DA(getColor(), &State);
        break;
      case 13:
        draw_ES_IST(getColor(), &State);
        draw_NOCH(getColor(), &State);
        draw_EIS(getColor(), &State);
        draw_DA(getColor(), &State);
        break;
      case 14:
        draw_ES_IST(getColor(), &State);
        draw_NIX(getColor(), &State);
        draw_DA(getColor(), &State);
        break;
      default:
        break;
      }
      ActualMin = Now.minute();
      }
    
  }

  /* Lauftext*/
  if(State.Mode == MODE_MARQUEE_TIME || State.Mode == MODE_MARQUEE_TEXT){
    String dataString;
    if(State.Mode == MODE_MARQUEE_TEXT){
      dataString = State.Marquee_Text;
    }else{
      dataString += String(Now.hour());
      dataString += ":";
      dataString += String(Now.minute());
      dataString += ":";
      dataString += String(Now.second());
    }
    matrix.fillScreen(0);
    matrix.setCursor(x, 2);
    //matrix.print(F("Leon!"));
    matrix.print(dataString);
    if(--x < -(int)(dataString.length() * 6)) {
      x = matrix.width();
      if(++pass >= 7) pass = 0;
      matrix.setTextColor(colors[pass]);
    }
    matrix.show();
  //delay(150);
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

    Serial.println("Wordclock");
    Serial.print(Now.minute());
    Serial.println(" Min");
    Serial.print(Now.hour());
    Serial.println(" hour");

    delay(100);
  }

}
