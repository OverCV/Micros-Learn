// dispenser.cpp
#include "dispenser.h"

Dispenser::Dispenser()
  : _dispensing(false) {}

void Dispenser::init() {
  // Inicializar todos los pines de relay configurados en el catálogo
  for (uint8_t i = 0; i < Product::getProductCount(); i++) {
    const Product::ProductInfo* info = Product::getProductInfo(i + 1);  // Asumiendo IDs 1-based
    if (info && info->available) {
      pinMode(info->relayPin, OUTPUT);
      digitalWrite(info->relayPin, LOW);
    }
  }
  reset();
}

void Dispenser::activateRelay(uint8_t pin) {
  digitalWrite(pin, HIGH);
  delay(DISPENSE_TIME);
  digitalWrite(pin, LOW);
  delay(DISPENSE_TIME);
}

void Dispenser::dispense(Product::ProductId id) {
  const Product::ProductInfo* info = Product::getProductInfo(id);
  if (info && info->available) {
    _dispensing = true;
    activateRelay(info->relayPin);
    _dispensing = false;
  }
}

void Dispenser::reset() {
  // Resetear todos los relays a su estado inicial
  for (uint8_t i = 0; i < Product::getProductCount(); i++) {
    const Product::ProductInfo* info = Product::getProductInfo(i + 1);  // Asumiendo IDs 1-based
    if (info && info->available) {
      digitalWrite(info->relayPin, LOW);
    }
  }
  _dispensing = false;
}