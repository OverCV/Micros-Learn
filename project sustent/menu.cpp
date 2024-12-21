// menu.cpp
#include "menu.h"

Menu::Menu()
  : _cartSize(0), _total(0) {}

void Menu::updateTotal() {
  _total = 0;
  for (uint8_t i = 0; i < _cartSize; i++) {
    const Product::ProductInfo* info = Product::getProductInfo(_cart[i].id);
    if (info) {
      _total += info->price * _cart[i].quantity;
    }
  }
}

int8_t Menu::findItemIndex(Product::ProductId id) const {
  for (uint8_t i = 0; i < _cartSize; i++) {
    if (_cart[i].id == id) {
      return i;
    }
  }
  return -1;  // No encontrado
}

bool Menu::addToCart(Product::ProductId id) {
  if (!Product::isValidProduct(id)) {
    return false;  // Producto inválido
  }

  int8_t index = findItemIndex(id);

  if (index != -1) {
    // Producto ya existe en el carrito
    _cart[index].quantity++;
  } else {
    // Nuevo producto
    if (_cartSize >= MAX_ITEMS) {
      return false;  // Carrito lleno
    }
    _cart[_cartSize].id = id;
    _cart[_cartSize].quantity = 1;
    _cartSize++;
  }

  updateTotal();
  return true;
}

bool Menu::removeFromCart(Product::ProductId id) {
  int8_t index = findItemIndex(id);

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

uint8_t Menu::getProductCount(Product::ProductId id) const {
  int8_t index = findItemIndex(id);
  return (index != -1) ? _cart[index].quantity : 0;
}

void Menu::clearCart() {
  _cartSize = 0;
  _total = 0;
}