// main.cpp
#include <Arduino.h>
#include "keypad.h"

const byte ROWS = 4;
const byte COLS = 4;
const char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '<', '0', '>', 'D' }
};
const byte rowPins[ROWS] = { 46, 47, 48, 49 };
const byte colPins[COLS] = { 50, 51, 52, 53 };

Keypad keypad(rowPins, colPins, keys);

void setup() {
  Serial.begin(9600);
  keypad.init();
}

void loop() {
  char key = keypad.getKey();

  if (key != '\0') {
    Serial.print("Tecla presionada: ");
    Serial.println(key);
  }
}

ISR(PCINT0_vect) {
  keypad.setKeyDetected(true);
}