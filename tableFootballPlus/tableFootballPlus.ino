/* 
 *  Code for the table football at USI, Lugano
 *  developed by Stefano Taillefert in August 2020
 *  for Arduino Uno.
 *  
 *  See https://github.com/USI-Showroom/TableFootballPlus
 *  
 *  Libraries used: FastLED (https://github.com/FastLED/FastLED)
 */

#include <FastLED.h>

#define NUM_LEDS 1
#define DEBUG

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
  #ifdef DEBUG
    Serial.begin(115200);
    Serial.print("Starting up...");
  #endif
  
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

  setDefaultLights();

  #ifdef DEBUG
    Serial.println(" done.");
  #endif
}

/*
 * Updates values and produces sound/light effects
 * if the white player scores
 */
void scoreEffectWhite() {  
  ++scoreWhite;

  #ifdef DEBUG
    Serial.println("White scored!");
    Serial.print("Score: ");
    Serial.print(scoreWhite);
    Serial.print("-");
    Serial.println(scoreYellow);
  #endif
  
  tone(BUZZER, 450, 400);
  delay(150);
  tone(BUZZER, 470, 400);

  delayedLoop(CRGB::White, 500);
}

/*
 * Updates values and produces sound/light effects
 * if the yellow player scores
 */
void scoreEffectYellow() {  
  ++scoreYellow;

  #ifdef DEBUG
    Serial.println("Yellow scored!");
    Serial.print("Score: ");
    Serial.print(scoreWhite);
    Serial.print("-");
    Serial.println(scoreYellow);
  #endif
  
  tone(BUZZER, 400, 400);
  delay(150);
  tone(BUZZER, 450, 400);

  delayedLoop(CRGB::Yellow, 500);
}

/*
 * Changes the color of the whole strip at once.
 */
void changeColor(CRGB newColor) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = newColor;
  }
  FastLED.show();
}

/*
 * Produces a ring effect with the given color and delay,
 * then return to default effect.
 */
void delayedLoop(CRGB newColor, int interval) {
  leds[0] = newColor;
  delay(interval);
  for (int i = 1; i < NUM_LEDS; ++i) {
    leds[i] = newColor;
    leds[i - 1] = CRGB::Black;
    delay(interval);
  }
  leds[NUM_LEDS - 1] = CRGB::Black;
  delay(interval);
  setDefaultLights();
}

/*
 * Sets the default idle effect.
 */
void setDefaultLights() {
  changeColor(CRGB::Green);
  //TODO fancier effect
}

void loop() {
  
  //Handle light change
  
  if (digitalRead(BTN_LIGHTS) == 1) {
    #ifdef DEBUG
      Serial.println("Toggling lights effects\n");
    #endif
    //Toggle lights
    lightsOn = !lightsOn;
  }

  if (lightsOn) {
    setDefaultLights();
  } else {
    changeColor(CRGB::Black);
  }

  //Handle score change

  if (digitalRead(BTN_SCORE_WHITE_UP) == 1 || digitalRead(SENSOR_WHITE) == 1) {
    scoreEffectWhite();
  }

  if (digitalRead(BTN_SCORE_YELLOW_UP) == 1 || digitalRead(SENSOR_YELLOW) == 1) {
    scoreEffectYellow();
  }

  if (digitalRead(BTN_SCORE_WHITE_DOWN) == 1) {
    --scoreWhite;
  }

  if (digitalRead(BTN_SCORE_YELLOW_DOWN) == 1) {
    --scoreYellow;
  }
}
