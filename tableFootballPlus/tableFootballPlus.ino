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
#include <avr/wdt.h>

#define DEBUG
#define DEFAULT_COLOR CRGB::Blue
#define MAX_SCORE 10

//LED strip references. Don't change them unless 
//you actually change or move the strip itself
#define NUM_LEDS 218
#define OFFSET_Y 87   //Position in the strip where the yellow counter starts
#define OFFSET_W 195
#define LED_STRIP WS2813
#define LED_MODE GRB

//Port numbers
#define BTN_RESET 2
#define BTN_SCORE_YELLOW_DOWN 3
#define BTN_SCORE_YELLOW_UP 4
#define BTN_SCORE_WHITE_DOWN 5
#define BTN_SCORE_WHITE_UP 6
#define BTN_LIGHTS 7
#define SENSOR_WHITE 8
#define SENSOR_YELLOW 9
#define LED_DATA 10
#define BUZZER 11

int scoreWhite = 0;
int scoreYellow = 0;
CRGB leds[NUM_LEDS];

/*
 * Plays a tone when one of the players scores.
 */
void playScoreTone() {
  #ifdef DEBUG
    tone(BUZZER, 450, 100);
    delay(150);
  #else
    tone(BUZZER, 450, 200);
    delay(130);
    tone(BUZZER, 550, 400);
    delay(220);
    tone(BUZZER, 450, 200);
    delay(130);
    tone(BUZZER, 550, 800);
    delay(150);
  #endif
}


/*
 * Plays a tone when a goal is removed.
 */
void playDownTone() {
  #ifdef DEBUG
    tone(BUZZER, 350, 100);
    delay(150);
  #else
    tone(BUZZER, 600, 400);
    delay(150);
    tone(BUZZER, 500, 200);
    delay(150);
    tone(BUZZER, 400, 200);
    delay(150);
  #endif
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
 * Produces a ring effect with the given color and optional delay.
 */
void delayedLoop(CRGB newColor, int interval = 0) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = newColor;  
    FastLED.show();
    delay(interval);
  }
}


/*
 * Sets the default idle light effect.
 * Each short side shows the goal count, while
 * the long ones have a fixed DEFAULT_COLOR.
 */
void setDefaultLights() {
  for (int i = 0; i < NUM_LEDS; ++i) {
    if (i < OFFSET_Y) {
      leds[i] = DEFAULT_COLOR;
    } else if (i <= OFFSET_Y + scoreYellow - 1) {
      leds[i] = CRGB::Yellow;
    } else if (i < OFFSET_Y + MAX_SCORE) {
      leds[i] = CRGB::Black;
    } else if (i < OFFSET_W) {
      leds[i] = DEFAULT_COLOR;
    } else if (i <= OFFSET_W + scoreWhite - 1) {
      leds[i] = CRGB::White;
    } else if (i < OFFSET_W + MAX_SCORE) {
      leds[i] = CRGB::Black;
    } else {
      leds[i] = DEFAULT_COLOR;
    }
  }
  FastLED.show();
}


void setup() {
  delay(2000);
  #ifdef DEBUG
    Serial.begin(115200);
    Serial.print("Starting up...");
  #endif

  pinMode(BTN_RESET, INPUT);
  pinMode(BTN_SCORE_YELLOW_DOWN, INPUT);
  pinMode(BTN_SCORE_YELLOW_UP, INPUT);
  pinMode(BTN_SCORE_WHITE_DOWN, INPUT);
  pinMode(BTN_SCORE_WHITE_UP, INPUT);
  pinMode(BTN_LIGHTS, INPUT);
  pinMode(SENSOR_WHITE, INPUT);
  pinMode(SENSOR_YELLOW, INPUT);
  pinMode(BUZZER, OUTPUT);  

  FastLED.addLeds<LED_STRIP, LED_DATA, LED_MODE>(leds, NUM_LEDS);
  delayedLoop(CRGB::Green);

  //Startup sound
  tone(BUZZER, 450, 400);    //450 MHz for 400 ms
  delay(150);
  tone(BUZZER, 500, 200);
  delay(150);
  tone(BUZZER, 600, 200);
  delay(150);

  #ifdef DEBUG
    Serial.println(" done.");
  #endif
}


void loop() {
  //Handle light change
  if (digitalRead(BTN_LIGHTS) == 1) {
    #ifdef DEBUG
      Serial.println("Toggling lights effects");
    #endif

    //TODO cycle colors
  }

  setDefaultLights();

  //Handle victory and score change
  if (scoreWhite == MAX_SCORE) {
    #ifdef DEBUG
      Serial.println("White won!");
    #endif
    delay(1000);
    changeColor(CRGB::White);
    delay(1000);
  } else if (scoreYellow == MAX_SCORE) {
    #ifdef DEBUG
      Serial.println("Yellow won!");
    #endif
    delay(1000);
    changeColor(CRGB::Yellow);
    delay(1000);
  } else {
    //White scored
    if (digitalRead(BTN_SCORE_WHITE_UP) == 1 || digitalRead(SENSOR_YELLOW) == 1) {
      ++scoreWhite;
  
      #ifdef DEBUG
        Serial.println("White scored!");
        Serial.print("Score: ");
        Serial.print(scoreWhite);
        Serial.print("-");
        Serial.println(scoreYellow);
      #endif
      
      playScoreTone();
      delayedLoop(CRGB::White, 10);
    }

    //Yellow scored
    if (digitalRead(BTN_SCORE_YELLOW_UP) == 1 || digitalRead(SENSOR_WHITE) == 1) {
      ++scoreYellow;
  
      #ifdef DEBUG
        Serial.println("Yellow scored!");
        Serial.print("Score: ");
        Serial.print(scoreWhite);
        Serial.print("-");
        Serial.println(scoreYellow);
      #endif
      
      playScoreTone();
      delayedLoop(CRGB::Yellow, 10);
    }
  
    if (digitalRead(BTN_SCORE_WHITE_DOWN) == 1) {
      if (scoreWhite > 0) {
        --scoreWhite;
        playDownTone();
  
        #ifdef DEBUG
          Serial.println("Decreasing white score");
          Serial.print("Score: ");
          Serial.print(scoreWhite);
          Serial.print("-");
          Serial.println(scoreYellow);
        #endif
      }    
    }
  
    if (digitalRead(BTN_SCORE_YELLOW_DOWN) == 1) {
      if (scoreYellow > 0) {
        --scoreYellow;
        playDownTone();
  
        #ifdef DEBUG
          Serial.println("Decreasing yellow score");
          Serial.print("Score: ");
          Serial.print(scoreWhite);
          Serial.print("-");
          Serial.println(scoreYellow);
        #endif
      }    
    }
  }

  if (digitalRead(BTN_RESET) == 1) {
    #ifdef DEBUG
      Serial.println("Reset");
    #endif
    //To reset the board, we set the watchdog to 15ms and then busy wait.
    //Sounds like a hack but apparently that's the way ¯\_(ツ)_/¯
    wdt_enable(WDTO_15MS);
    while(1) {}
  }

  delay(200);
}
