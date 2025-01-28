// keypad.cpp
#include "keypad.h"

Keypad::Keypad(const byte* rowPins, const byte* colPins, const char (*keys)[4]) {
  _rowPins = rowPins;
  _colPins = colPins;
  _keys = keys;
  _keyDetected = false;
  _currentRow = 0;
}

void Keypad::init() {
  // Configurar filas como salidas
  for (byte i = 0; i < 4; i++) {
    pinMode(_rowPins[i], OUTPUT);
    digitalWrite(_rowPins[i], HIGH);
  }

  // Configurar columnas como entradas con pull-up
  for (byte i = 0; i < 4; i++) {
    pinMode(_colPins[i], INPUT_PULLUP);
  }

  // Configurar interrupciones PCINT0:3 para pines 50-53
  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3);
}

char Keypad::getKey() {
  unsigned long currentTime = millis();

  // Verificar si el debounce permite procesar la tecla
  if ((currentTime - _lastDebounceTime) < DEBOUNCE_DELAY) {
    return '\0';  // Ignorar pulsaciones rápidas
  }

  digitalWrite(_rowPins[_currentRow], HIGH);
  _currentRow = (_currentRow + 1) % 4;
  digitalWrite(_rowPins[_currentRow], LOW);

  if (_keyDetected) {
    for (byte columna = 0; columna < 4; columna++) {
      if (digitalRead(_colPins[columna]) == LOW) {
        _keyDetected = false;
        _lastDebounceTime = currentTime;  // Actualizar tiempo de debounce
        return _keys[_currentRow][columna];
      }
    }
    _keyDetected = false;
  }

  delayMicroseconds(500);
  return '\0';
}
