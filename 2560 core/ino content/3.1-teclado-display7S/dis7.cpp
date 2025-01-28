// dis7.cpp
#include "dis7.h"

const uint8_t Display7::SEGMENT_PATTERNS[16] = {
  0b00111111,  // 0
  0b00000110,  // 1
  0b01011011,  // 2
  0b01001111,  // 3
  0b01100110,  // 4
  0b01101101,  // 5
  0b01111101,  // 6
  0b00000111,  // 7
  0b01111111,  // 8
  0b01101111,  // 9
  0b01110111,  // A
  0b01111100,  // b
  0b00111001,  // C
  0b01011110,  // d
  0b01111001,  // E
  0b01110001,  // F
};

Display7::Display7(uint8_t pinA, uint8_t pinB, uint8_t pinC,
                   uint8_t pinD, uint8_t pinE, uint8_t pinF,
                   uint8_t pinG, bool isCommonCathode)
  : _pinA(pinA),
    _pinB(pinB),
    _pinC(pinC),
    _pinD(pinD),
    _pinE(pinE),
    _pinF(pinF),
    _pinG(pinG),
    _isCommonCathode(isCommonCathode) {
}

void Display7::init() {
  pinMode(_pinA, OUTPUT);
  pinMode(_pinB, OUTPUT);
  pinMode(_pinC, OUTPUT);
  pinMode(_pinD, OUTPUT);
  pinMode(_pinE, OUTPUT);
  pinMode(_pinF, OUTPUT);
  pinMode(_pinG, OUTPUT);
  clear();
}

void Display7::writeSegments(uint8_t pattern) {
  if (!_isCommonCathode) {
    pattern = ~pattern;
  }
  digitalWrite(_pinA, (pattern >> 0) & 0x01);
  digitalWrite(_pinB, (pattern >> 1) & 0x01);
  digitalWrite(_pinC, (pattern >> 2) & 0x01);
  digitalWrite(_pinD, (pattern >> 3) & 0x01);
  digitalWrite(_pinE, (pattern >> 4) & 0x01);
  digitalWrite(_pinF, (pattern >> 5) & 0x01);
  digitalWrite(_pinG, (pattern >> 6) & 0x01);
}

void Display7::showNumber(uint8_t number) {
  if (number > 9) {
    clear();
    return;
  }
  writeSegments(SEGMENT_PATTERNS[number]);
}

void Display7::clear() {
  writeSegments(_isCommonCathode ? 0x00 : 0xFF);
}

void Display7::test() {
  // Test individual segments
  for (uint8_t i = 0; i < 7; i++) {
    writeSegments(1 << i);
    delay(500);
  }
  // Test all numbers
  for (uint8_t i = 0; i <= 9; i++) {
    showNumber(i);
    delay(500);
  }

  clear();
}