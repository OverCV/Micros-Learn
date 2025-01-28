#include <avr/io.h>
#include <avr/interrupt.h>
#include "dis7.h"
#include "keypad.h"

// Pines del display de 7 segmentos
const uint8_t PIN_A = 22;
const uint8_t PIN_B = 23;
const uint8_t PIN_C = 24;
const uint8_t PIN_D = 25;
const uint8_t PIN_E = 26;
const uint8_t PIN_F = 27;
const uint8_t PIN_G = 28;

// Pines del teclado
const byte ROW_PINS[4] = { 46, 47, 48, 49 };
const byte COL_PINS[4] = { 50, 51, 52, 53 };
const char KEYS[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '<', '0', '>', 'D' }
};

Display7 display(PIN_A, PIN_B, PIN_C, PIN_D, PIN_E, PIN_F, PIN_G, true);
Keypad keypad(ROW_PINS, COL_PINS, KEYS);

volatile char pressedKey = '\0';

void setup() {
  display.init();
  keypad.init();
}

void loop() {
  char key = keypad.getKey();
  if (key != '\0') {
    pressedKey = key;
  }

  if (pressedKey != '\0') {
    showOnDisplay(pressedKey);
  }
}

void showOnDisplay(char key) {
  if (key >= '0' && key <= '9') {
    display.showNumber(key - '0');
  } else if (key >= 'A' && key <= 'F') {
    display.writeSegments(Display7::SEGMENT_PATTERNS[10 + (key - 'A')]);
  } else {
    display.clear();
  }
}

ISR(PCINT0_vect) {
  keypad.setKeyDetected(true);
}