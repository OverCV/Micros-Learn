// product.h
#ifndef PRODUCT_H
#define PRODUCT_H

#include <Arduino.h>

class Product {
public:
  // Tipos de productos disponibles
  enum Type {
    WATER,
    SODA
  };

private:
  Type _type;
  uint16_t _price;
  String _name;

public:
  // Constructor
  Product(Type type, uint16_t price, String name);

  // Getters
  Type getType() const {
    return _type;
  }
  uint16_t getPrice() const {
    return _price;
  }
  String getName() const {
    return _name;
  }

  // Método estático para obtener precio por tipo
  static uint16_t getPriceByType(Type type);
};

#endif
