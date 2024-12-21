// product.h
#ifndef PRODUCT_H
#define PRODUCT_H

#include <Arduino.h>

class Product {
public:
  typedef uint8_t ProductId;

  struct ProductInfo {
    ProductId id;
    const char* name;
    uint16_t price;
    uint8_t relayPin;
    bool available;
    char displayKey;  // Nueva propiedad para la letra de display
  };
  ;

  static const ProductInfo* getProductInfo(ProductId id);
  static uint8_t getProductCount();
  static bool isValidProduct(ProductId id);

private:
  static const ProductInfo PRODUCT_CATALOG[];
  static const uint8_t PRODUCT_COUNT;
};

#endif