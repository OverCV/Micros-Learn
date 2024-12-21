// menu.h
#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "product.h"

class Menu {
public:
  struct CartItem {
    Product::ProductId id;  // Ahora usamos ProductId en lugar de Type
    uint8_t quantity;
  };

private:
  static const uint8_t MAX_ITEMS = 10;  // Máximo número de items diferentes en el carrito

  CartItem _cart[MAX_ITEMS];  // Array estático para items
  uint8_t _cartSize;          // Número actual de items diferentes
  uint16_t _total;            // Total del carrito

  // Métodos privados auxiliares
  void updateTotal();
  int8_t findItemIndex(Product::ProductId id) const;

public:
  Menu();

  // Gestión del carrito
  bool addToCart(Product::ProductId id);
  bool removeFromCart(Product::ProductId id);

  // Consultas
  uint8_t getProductCount(Product::ProductId id) const;
  uint16_t getTotal() const {
    return _total;
  }
  bool hasProducts() const {
    return _cartSize > 0;
  }
  const CartItem* getCart() const {
    return _cart;
  }
  uint8_t getSize() const {
    return _cartSize;
  }

  // Reinicio
  void clearCart();
};

#endif