#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#include "routine.h"
#include "settings.h"
#include "timer.h"

static void checkConnection();
static void alive();
static void bright();
static void recieve();

static constexpr char Prefix[] = {'A', 'd', 'a'};
static boolean stripState = true;

static Adafruit_NeoPixel strip = Adafruit_NeoPixel(
    Settings::NumberOfLeds, Settings::DiPin, NEO_RGB + NEO_KHZ800);

static Routine aliveRoutine(1000, alive);
static Routine brightRoutine(100, bright);
static Routine recvRoutine(Routine::Everytime, recieve);
static Timer checkConnectionTimer(Settings::OffTime);

void alive() {
  constexpr int alivePin = 1;
  static bool init = false;
  if (init == false) {
    pinMode(alivePin, OUTPUT);
    init = true;
  }
  auto state = digitalRead(alivePin);
  digitalWrite(alivePin, state == HIGH ? LOW : HIGH);
}

void bright() {
  constexpr unsigned char maxBright = 255;
  constexpr unsigned char minBright = 50;
  static int newBrightFilter;
  int newBright = map(analogRead(Settings::PhotoResPin), 0,
                      Settings::BrightConstant, minBright, maxBright);
  newBright = constrain(newBright, minBright, maxBright);
  newBrightFilter =
      newBrightFilter * Settings::Coef + newBright * (1 - Settings::Coef);
  strip.setBrightness(newBrightFilter);
}

void checkConnection() {
  if (stripState) {
    if (checkConnectionTimer.isReady() == true) {
      stripState = false;
      strip.clear();
      strip.show();
    }
  }
}

void recieve() {
  if (stripState == false) stripState = true;
  checkConnectionTimer.reset();

  unsigned char i;
  for (i = 0; i < sizeof(Prefix); ++i) {
  waitLoop:
    while (Serial.available() < 1) checkConnection();
    if (Prefix[i] == Serial.read()) continue;
    i = 0;
    goto waitLoop;
  }

  while (Serial.available() < 1) checkConnection();
  unsigned char hi = Serial.read();
  while (Serial.available() < 1) checkConnection();
  unsigned char lo = Serial.read();
  while (Serial.available() < 1) checkConnection();
  unsigned char chk = Serial.read();
  if (chk != (hi ^ lo ^ 0x55)) {
    i = 0;
    goto waitLoop;
  }

  for (int i = 0; i < Settings::NumberOfLeds; i++) {
    while (Serial.available() < 1) checkConnection();
    unsigned char r = Serial.read();
    while (Serial.available() < 1) checkConnection();
    unsigned char g = Serial.read();
    while (Serial.available() < 1) checkConnection();
    unsigned char b = Serial.read();
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
}

void setup() {
  strip.begin();
  strip.show();
  if (Settings::StartBlink) {
    strip.Color(255, 0, 0);
    strip.show();
    delay(500);
    strip.Color(0, 255, 0);
    strip.show();
    delay(500);
    strip.Color(0, 0, 255);
    strip.show();
    delay(500);
    strip.Color(0, 0, 0);
    strip.show();
  }

  Serial.begin(115200);
  Serial.print(Prefix);
  Serial.print('\n');

  aliveRoutine.run();
  brightRoutine.run();
  recvRoutine.run();
  checkConnectionTimer.start();
}

void loop() {
  aliveRoutine.process();
  if (Settings::AutoBright) brightRoutine.process();
  recvRoutine.process();
}