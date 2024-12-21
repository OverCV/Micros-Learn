// product.cpp
#include "product.h"
#include "pins.h"

extern const uint8_t RELAY_WATER;  // Declaramos como externos los pines
extern const uint8_t RELAY_SODA;   // que vienen del .ino
extern const uint8_t RELAY_PONY;   // que vienen del .ino

const Product::ProductInfo Product::PRODUCT_CATALOG[] = {
  { 1, "Agua", 500, RELAY_WATER, true, 'A' },
  { 2, "Soda", 700, RELAY_SODA, true, 'B' },
  { 3, "Pony", 1000, RELAY_PONY, true, 'C' },
};

const uint8_t Product::PRODUCT_COUNT = sizeof(PRODUCT_CATALOG) / sizeof(ProductInfo);

const Product::ProductInfo* Product::getProductInfo(ProductId id) {
  for (uint8_t i = 0; i < PRODUCT_COUNT; i++) {
    if (PRODUCT_CATALOG[i].id == id) {
      return &PRODUCT_CATALOG[i];
    }
  }
  return nullptr;
}

uint8_t Product::getProductCount() {
  return PRODUCT_COUNT;
}

bool Product::isValidProduct(ProductId id) {
  return getProductInfo(id) != nullptr;
}