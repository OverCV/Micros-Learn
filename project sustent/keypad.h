// keypad.h
#ifndef KEYPAD_H
#define KEYPAD_H

#include <Arduino.h>

class Keypad {
private:
  // Evitar altas velocidades en milisegundos para interacción humano-máquina
  unsigned long _lastDebounceTime;
  static const unsigned long DEBOUNCE_DELAY = 200;

  const byte* _rowPins;
  const byte* _colPins;
  const char (*_keys)[4];
  volatile bool _keyDetected;
  volatile byte _currentRow;

public:
  Keypad(const byte* rowPins, const byte* colPins, const char (*keys)[4]);
  void init();
  char getKey();
  void setKeyDetected(bool detected) {
    _keyDetected = detected;
  }
  byte getCurrentRow() const {
    return _currentRow;
  }
  void setCurrentRow(byte row) {
    _currentRow = row;
  }
};

#endif