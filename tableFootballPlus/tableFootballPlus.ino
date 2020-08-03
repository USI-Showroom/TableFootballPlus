/* 
 *  Code for the table football at USI, Lugano
 *  by Stefano Taillefert, August 2020
 *  
 *  Libraries used: FastLED (https://github.com/FastLED/FastLED)
 */

#include <FastLED.h>

#define NUM_LEDS 1

//Port numbers
#define BUZZER 0
#define SENSOR_WHITE 0
#define SENSOR_YELLOW 0
#define BTN_SCORE_WHITE_UP 0
#define BTN_SCORE_WHITE_DOWN 0
#define BTN_SCORE_YELLOW_UP 0
#define BTN_SCORE_YELLOW_DOWN 0
#define BTN_LIGHTS 0
#define LED_DATA 0

bool lightsOn = true;
int scoreWhite = 0;
int scoreYellow = 0;
CRGB leds[NUM_LEDS];

void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(SENSOR_WHITE, INPUT);
  pinMode(SENSOR_YELLOW, INPUT);
  pinMode(BTN_SCORE_WHITE_UP, INPUT);
  pinMode(BTN_SCORE_WHITE_DOWN, INPUT);
  pinMode(BTN_SCORE_YELLOW_UP, INPUT);
  pinMode(BTN_SCORE_YELLOW_DOWN, INPUT);
  pinMode(BTN_LIGHTS, INPUT);

  FastLED.addLeds<WS2813, LED_DATA, RGB>(leds, NUM_LEDS);

  //Startup sound
  tone(BUZZER, 450, 400);    //450 MHz for 400 ms
  delay(150);
  tone(BUZZER, 500, 200);
  delay(150);
  tone(BUZZER, 600, 200);

  leds[0] = CRGB::Green;
  FastLED.show();
}

void scoreEffectWhite() {
  scoreWhite++;
  
  tone(BUZZER, 450, 400);
  delay(150);
  tone(BUZZER, 470, 400);

  leds[0] = CRGB::White;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Black;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::White;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Green;
  FastLED.show();
  delay(500);
}

void scoreEffectYellow() {
  scoreYellow++;
  
  tone(BUZZER, 400, 400);
  delay(150);
  tone(BUZZER, 450, 400);

  leds[0] = CRGB::Yellow;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Black;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Yellow;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Green;
  FastLED.show();
  delay(500);
}

void loop() {
  if (digitalRead(BTN_LIGHTS) == 1) {
    //Toggle lights
    lightsOn = !lightsOn;
  }

  if (lightsOn) {
    //TODO turn on or off lights
  } else {
    
  }

  if (digitalRead(BTN_SCORE_WHITE_UP) == 1 || digitalRead(SENSOR_WHITE) == 1) {
    scoreEffectWhite();
  }

  if (digitalRead(BTN_SCORE_YELLOW_UP) == 1 || digitalRead(SENSOR_YELLOW) == 1) {
    scoreEffectYellow();
  }
}
