// This #include statement was automatically added by the Spark IDE.
#include "neopixel/neopixel.h"


#define PIN    D0
#define N_LEDS 50
int inPin = D3;
int val = 0;
int tam = 0;
bool hro = false;
int r = 0;
int g = 255;
int b = 0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, WS2812B);

void setup() {
  Serial.begin(9600);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(inPin, INPUT);
  strip.begin();
}

void loop() {

  val = digitalRead(inPin);
  if (val == 1 && hro == false && tam == 0) {     // 1 ON
    for (int i = 0; i <= 15; i++) {
      strip.setPixelColor(i, r, g, b);
      strip.setPixelColor(i-1, 0);
      strip.show();
      delay(100);
    }
    tam = 1;
    hro = true;
    digitalWrite(D1, HIGH);
  }
  Serial.println(val);


  if (tam == 1 && val == 0 && hro == true) {      // 1 OFF
    digitalWrite(D1, LOW);
    for (int i = 15; i >= 0; i--) {
      strip.setPixelColor(i, r, g, b);
      strip.setPixelColor(i+1, 0);
      strip.show();
      delay(100);
      tam = 2;
      hro = false;
    }
  }



  if (tam == 2 && val == 1 && hro == false) {       //12-3 ON
    for (int i = 0; i <= 30; i++) {
      strip.setPixelColor(i, r, g, b);
      strip.setPixelColor(i-1, 0);
      strip.show();
      delay(50);
      tam = 3;
      hro = true;
      if (i == 15) {
        digitalWrite(D1, HIGH);
      }
    }
    digitalWrite(D2, HIGH);
    delay(500);
    for (int i = 30; i <= 45; i++) {
      strip.setPixelColor(i, r, g, b);
      strip.setPixelColor(i-1, 0);
      strip.show();
      delay(50);
        if(i==40){
            Spark.publish("middleThing","eventDetails");
        }
    }
  }


  if (tam == 3 && val == 0 && hro == true) {      //321 OFF
    for (int i = 45; i >= 0; i--) {
      strip.setPixelColor(i, r, g, b);
      strip.setPixelColor(i+1, 0);
      strip.show();
      delay(50);
      tam = 0;
      hro = false;
      if(i==30){
        digitalWrite(D2, LOW);        
      }
      if(i==15){
        digitalWrite(D1, LOW);
      }
    }
  }
}
