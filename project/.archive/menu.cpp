// menu.cpp
#include "menu.h"

Menu::Menu()
  : _cartSize(0), _total(0) {}

void Menu::updateTotal() {
  _total = 0;
  for (uint8_t i = 0; i < _cartSize; i++) {
    _total += Product::getPriceByType(_cart[i].type) * _cart[i].quantity;
  }
}

int8_t Menu::findItemIndex(Product::Type type) const {
  for (uint8_t i = 0; i < _cartSize; i++) {
    if (_cart[i].type == type) {
      return i;
    }
  }
  return -1;  // No encontrado
}

bool Menu::addToCart(Product::Type type) {
  int8_t index = findItemIndex(type);

  if (index != -1) {
    // Producto ya existe en el carrito
    _cart[index].quantity++;
  } else {
    // Nuevo producto
    if (_cartSize >= MAX_ITEMS) {
      return false;  // Carrito lleno
    }
    _cart[_cartSize].type = type;
    _cart[_cartSize].quantity = 1;
    _cartSize++;
  }

  updateTotal();
  return true;
}

bool Menu::removeFromCart(Product::Type type) {
  int8_t index = findItemIndex(type);

  if (index != -1) {
    if (--_cart[index].quantity == 0) {
      // Eliminar el producto moviendo los demás una posición
      for (uint8_t i = index; i < _cartSize - 1; i++) {
        _cart[i] = _cart[i + 1];
      }
      _cartSize--;
    }
    updateTotal();
    return true;
  }
  return false;
}

uint8_t Menu::getProductCount(Product::Type type) const {
  int8_t index = findItemIndex(type);
  return (index != -1) ? _cart[index].quantity : 0;
}

void Menu::clearCart() {
  _cartSize = 0;
  _total = 0;
}
