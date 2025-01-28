// dis7.h
#ifndef DISPLAY7_H
#define DISPLAY7_H

#include <Arduino.h>

class Display7 {
private:
  // Pines para cada segmento
  const uint8_t _pinA;
  const uint8_t _pinB;
  const uint8_t _pinC;
  const uint8_t _pinD;
  const uint8_t _pinE;
  const uint8_t _pinF;
  const uint8_t _pinG;
  const bool _isCommonCathode;

public:
  Display7(uint8_t pinA, uint8_t pinB, uint8_t pinC,
           uint8_t pinD, uint8_t pinE, uint8_t pinF,
           uint8_t pinG, bool isCommonCathode = true);

  // Patrones de bits para cada número (0-9)
  static const uint8_t SEGMENT_PATTERNS[16];
  // Método privado para actualizar los segmentos
  void writeSegments(uint8_t pattern);

  void init();
  void showNumber(uint8_t number);
  void clear();
  void test();
};

#endif