// dispenser.h
#ifndef DISPENSER_H
#define DISPENSER_H

#include <Arduino.h>
#include "product.h"

class Dispenser {
private:
  bool _dispensing;
  static const uint16_t DISPENSE_TIME = 500;

  void activateRelay(uint8_t pin);

public:
  Dispenser();  // Ya no necesitamos pasar los relays en el constructor

  void init();
  void dispense(Product::ProductId id);  // Cambiamos a usar ProductId
  bool isDispensing() const {
    return _dispensing;
  }
  void reset();
};

#endif