// dispenser.cpp
#include "dispenser.h"

Dispenser::Dispenser(uint8_t relayWater, uint8_t relaySoda)
  : RELAY_WATER(relayWater), RELAY_SODA(relaySoda),
    _dispensing(false) {}

void Dispenser::init() {
  pinMode(RELAY_WATER, OUTPUT);
  pinMode(RELAY_SODA, OUTPUT);
  for (uint8_t i = 0; i < 3; i++) {
    pinMode(LED_COIN[i], OUTPUT);
  }
  reset();
}

void Dispenser::activateRelay(uint8_t pin) {
  digitalWrite(pin, HIGH);
  delay(DISPENSE_TIME);
  digitalWrite(pin, LOW);
  delay(DISPENSE_TIME);
}

void Dispenser::dispense(Product::Type type) {

  switch (type) {
    case Product::WATER:
      activateRelay(RELAY_WATER);
      break;
    case Product::SODA:
      activateRelay(RELAY_SODA);
      break;
  }
}

// void Dispenser::blinkCoinLED(uint8_t coinIndex) {
//   if (coinIndex < 3) {
//     digitalWrite(LED_COIN[coinIndex], HIGH);
//     delay(100);
//     digitalWrite(LED_COIN[coinIndex], LOW);
//     delay(100);
//   }
// }

void Dispenser::reset() {
  digitalWrite(RELAY_WATER, LOW);
  digitalWrite(RELAY_SODA, LOW);
  for (uint8_t i = 0; i < 3; i++) {
    digitalWrite(LED_COIN[i], LOW);
  }
  _dispensing = false;
}
