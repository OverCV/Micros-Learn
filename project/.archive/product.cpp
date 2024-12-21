// product.cpp
#include "product.h"

Product::Product(Type type, uint16_t price, String name)
  : _type(type), _price(price), _name(name) {}

uint16_t Product::getPriceByType(Type type) {
  switch (type) {
    case WATER:
      return 500;
    case SODA:
      return 700;
    default:
      return 0;
  }
}
