#include "neopixel/neopixel.h"

#define LED_PIN D0
#define BUTTON_PIN D2
#define RELAY_A D4
#define RELAY_B D5
#define RELAY_C D6

int lastButtonState = 0;
bool hasRunOnce = false;

int sectionPixelCount_A = 6;
int sectionPixelCount_B = 5;
int sectionPixelCount_C = 7;
int pixelCount = sectionPixelCount_A + sectionPixelCount_B + sectionPixelCount_C;

int pixelSpeed = 100;
int sectionDelay = 1000;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, LED_PIN, WS2812B);
uint32_t pixelColor = strip.Color(0, 255, 0);

void setup() {
  pinMode(RELAY_A, OUTPUT);
  pinMode(RELAY_B, OUTPUT);
  pinMode(RELAY_C, OUTPUT);

  pinMode(BUTTON_PIN, INPUT);
  strip.begin();
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  if(buttonState != lastButtonState){

    // first time turning switch on, turn on section A
    if(buttonState == 1 && hasRunOnce == false) {
      for (int i = 0; i <= sectionPixelCount_A; i++) {
        strip.setPixelColor(i, pixelColor);
        strip.setPixelColor(i-1, 0);
        strip.show();
        delay(pixelSpeed);
      }

      hasRunOnce = true;
      digitalWrite(RELAY_A, HIGH);
    }

    // first time turning switch off, turn off section A
    if(buttonState == 0 && hasRunOnce == true) {
      digitalWrite(RELAY_A, LOW);

      for (int i = sectionPixelCount_A; i >= 0; i--) {
        strip.setPixelColor(i, pixelColor);
        strip.setPixelColor(i+1, 0);
        strip.show();
        delay(pixelSpeed);
      }

      hasRunOnce = false;
    }

    // second time turning switch on, turn on section A, then B, then C
    if(buttonState == 1 && hasRunOnce == false) {
      for (int i = 0; i <= (sectionPixelCount_A + sectionPixelCount_B); i++) {
        strip.setPixelColor(i, pixelColor);
        strip.setPixelColor(i-1, 0);
        strip.show();
        delay(pixelSpeed);
        if (i == sectionPixelCount_A) {
          digitalWrite(RELAY_A, HIGH);
        }
      }

      digitalWrite(RELAY_B, HIGH);
      delay(sectionDelay);

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

      hasRunOnce = true;
    }

    // second time turning switch on, turn off section A, then B, then C
    if(buttonState == 0 && hasRunOnce == true) {
      for (int i = 45; i >= 0; i--) {
        strip.setPixelColor(i, pixelColor);
        strip.setPixelColor(i+1, 0);
        strip.show();
        delay(pixelSpeed);

        hasRunOnce = false;

        if(i==sectionPixelCount_C){ digitalWrite(RELAY_C, LOW); }
        if(i==sectionPixelCount_B){ digitalWrite(RELAY_B, LOW); }
        if(i==sectionPixelCount_A){ digitalWrite(RELAY_A, LOW); }
      }
    }
  }

  lastButtonState = buttonState;
}
