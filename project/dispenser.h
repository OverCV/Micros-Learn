// dispenser.h
#ifndef DISPENSER_H
#define DISPENSER_H

#include <Arduino.h>
#include "product.h"

class Dispenser {
private:
  const uint8_t RELAY_WATER;
  const uint8_t RELAY_SODA;
  const uint8_t LED_COIN[3];
  bool _dispensing;
  static const uint16_t DISPENSE_TIME = 500;

  void activateRelay(uint8_t pin);

public:
  Dispenser(uint8_t relayWater, uint8_t relaySoda);

  void init();
  void dispense(Product::Type type);
  bool isDispensing() const {
    return _dispensing;
  }
  void blinkCoinLED(uint8_t coinIndex);
  void reset();
};

#endif