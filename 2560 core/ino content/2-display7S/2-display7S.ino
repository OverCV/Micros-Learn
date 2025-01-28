// main.cpp
#include <Arduino.h>
#include "dis7.h"

// Pines del display de 7 segmentos
const uint8_t PIN_A = 22;
const uint8_t PIN_B = 23;
const uint8_t PIN_C = 24;
const uint8_t PIN_D = 25;
const uint8_t PIN_E = 26;
const uint8_t PIN_F = 27;
const uint8_t PIN_G = 28;

Display7 display(PIN_A, PIN_B, PIN_C, PIN_D, PIN_E, PIN_F, PIN_G, true);

void setup() {
  display.init();
}

void loop() {
  // Realizar conteo del 0 al 9
  for (uint8_t i = 0; i <= 9; i++) {
    display.showNumber(i);
    delay(1000);
  }
}