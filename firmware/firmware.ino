#include "neopixel/neopixel.h"

#define LED_PIN D0
#define BUTTON_PIN D2
#define RELAY_A D4
#define RELAY_B D5
#define RELAY_C D6

// flow
int lastButtonState = 0;
int step = 0;

// pixel setting
int sectionPixelCount_A = 6;
int sectionPixelCount_B = 5;
int sectionPixelCount_C = 7;
int pixelCount = sectionPixelCount_A + sectionPixelCount_B + sectionPixelCount_C;

// speed configuration
int pixelSpeed = 100;
int sectionDelay = 750;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, LED_PIN, WS2812B);
uint32_t pixelColor = strip.Color(255, 0, 0);

void setup() {
  pinMode(RELAY_A, OUTPUT);
  pinMode(RELAY_B, OUTPUT);
  pinMode(RELAY_C, OUTPUT);

  pinMode(BUTTON_PIN, INPUT);
  strip.begin();

  for(int i=0; i<pixelCount; i++){
    strip.setPixelColor(i, 0);
  }
  strip.show();
}


void lightSectionA(){
  for (int i = 0; i < sectionPixelCount_A; i++) {
    strip.setPixelColor(i, pixelColor);
    strip.setPixelColor(i-1, 0);
    strip.show();
    delay(pixelSpeed);
  }
  strip.setPixelColor(sectionPixelCount_A-1, 0);
  strip.show();
  delay(pixelSpeed);
  digitalWrite(RELAY_A, HIGH);
}


void lightSectionB(){
  for (int i = sectionPixelCount_A; i <= (sectionPixelCount_A + sectionPixelCount_B - 1); i++) {
    strip.setPixelColor(i, pixelColor);
    strip.setPixelColor(i-1, 0);
    strip.show();
    delay(pixelSpeed);
  }
  strip.setPixelColor((sectionPixelCount_A + sectionPixelCount_B-1), 0);
  strip.show();
  delay(pixelSpeed);
  digitalWrite(RELAY_B, HIGH);
}


void lightSectionC(){
  for (int i = (sectionPixelCount_A + sectionPixelCount_B); i <= pixelCount; i++) {
    strip.setPixelColor(i, pixelColor);
    strip.setPixelColor(i-1, 0);
    strip.show();
    delay(pixelSpeed);

    if(i==pixelCount){
      Spark.publish("notifyWatch", "deets");
      digitalWrite(RELAY_C, HIGH);
    }
  }
}


void dimSectionA(){
  digitalWrite(RELAY_A, LOW);
  for (int i = (sectionPixelCount_A-1); i >= -1; i--) {
    strip.setPixelColor(i, pixelColor);
    strip.setPixelColor(i+1, 0);
    strip.show();
    delay(pixelSpeed);
  }
}


void dimSectionB(){
  digitalWrite(RELAY_B, LOW);
  for (int i = (sectionPixelCount_B-1); i >= -1; i--) {
    int pixelId = sectionPixelCount_A + i;
    strip.setPixelColor(pixelId, pixelColor);
    strip.setPixelColor(pixelId+1, 0);
    strip.show();
    delay(pixelSpeed);
  }
}


void dimSectionC(){
  digitalWrite(RELAY_C, LOW);
  for (int i = (sectionPixelCount_C-1); i >= -1; i--) {
    int pixelId = sectionPixelCount_A + sectionPixelCount_B + i;
    strip.setPixelColor(pixelId, pixelColor);
    strip.setPixelColor(pixelId+1, 0);
    strip.show();
    delay(pixelSpeed);
  }
}


void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  if(buttonState != lastButtonState){
    // first time turning switch on, turn on section A
    if(buttonState == 1 && step == 0) {
      lightSectionA();
      step++;
    }

    // first time turning switch off, turn off section A
    if(buttonState == 0 && step == 1) {
      dimSectionA();
      step++;
    }

    // second time turning switch on, turn on section A, then B, then C
    if(buttonState == 1 && step == 2) {
      lightSectionA();
      delay(sectionDelay);
      lightSectionB();
      delay(sectionDelay);
      lightSectionC();
      step++;
    }

    // second time turning switch on, turn off section A, then B, then C
    if(buttonState == 0 && step == 3) {
      dimSectionC();
      dimSectionB();
      dimSectionA();
      step = 0;
    }
  }

  lastButtonState = buttonState;
}
