// screen.cpp
#include "screen.h"

Screen::Screen(Display* lcd)
  : _lcd(lcd) {}

void Screen::showWelcome() {
  _lcd->clear();
  _lcd->setCursor(0, 0);
  _lcd->print("Bienvenido!");
  _lcd->setCursor(0, 1);
  _lcd->print("[>] Productos");
}

void Screen::showAdditionOptions() {
  _lcd->clear();
  _lcd->setCursor(0, 0);
  _lcd->print("Vista productos:");
  _lcd->setCursor(0, 1);
  _lcd->print("[<]Del  [>]Pay");
  delay(1000);
}

void Screen::showDeletionOptions() {
  _lcd->clear();
  _lcd->setCursor(0, 0);
  _lcd->print("Vista borrar:");
  _lcd->setCursor(0, 1);
  _lcd->print("[<]Add  [>]Pay");
  delay(1000);
}

void Screen::showCart(const Menu* menu) {
  _currentMenu = menu;           // Guardar referencia al menú
  _lastRotationTime = millis();  // Resetear el tiempo de rotación

  _lcd->clear();
  uint8_t displayedLines = 0;

  // Mostrar dos productos comenzando desde el índice actual
  for (uint8_t i = 0; i < min(2, Product::getProductCount()); i++) {
    uint8_t productIndex = (_startIndex + i) % Product::getProductCount();
    const Product::ProductInfo* info = Product::getProductInfo(productIndex + 1);

    if (info && info->available) {
      uint8_t quantity = menu->getProductCount(info->id);
      printProductLine(info, quantity, displayedLines++);
    }
  }
}

void Screen::updateRotation() {
  if (!_currentMenu || Product::getProductCount() <= 2) return;

  unsigned long currentTime = millis();
  if (currentTime - _lastRotationTime >= ROTATION_INTERVAL) {
    // Actualizar índice
    _startIndex = (_startIndex + 1) % Product::getProductCount();

    // Redibujar la pantalla
    showCart(_currentMenu);

    _lastRotationTime = currentTime;
  }
}

void Screen::printProductLine(const Product::ProductInfo* info, uint8_t quantity, uint8_t line) {
  char buffer[6];  // Para convertir números a texto
  _lcd->setCursor(0, line);

  // Imprimir cada carácter individualmente
  _lcd->writeChar('[');
  _lcd->writeChar('A' + (info->id - 1));  // A para id 1, B para id 2, etc.
  _lcd->writeChar(']');
  _lcd->writeChar(' ');

  _lcd->print(info->name);
  _lcd->print(": ");
  dtostrf(quantity, 2, 0, buffer);
  _lcd->print(buffer);
}

void Screen::showConfirmation(uint16_t totalAmount) {
  _lcd->clear();
  _lcd->setCursor(0, 0);
  char buffer[6];
  dtostrf(totalAmount, 2, 0, buffer);
  _lcd->print("Pagar $");
  _lcd->print(buffer);
  _lcd->setCursor(0, 1);
  _lcd->print("[<]Atras [>]Siga");
}

void Screen::showPayment(uint16_t required, uint16_t current) {
  if (required == lastRequired && current == lastCurrent) {
    return;  // No hay cambios que mostrar
  }

  lastRequired = required;
  lastCurrent = current;

  char buffer[9];
  _lcd->clear();

  _lcd->setCursor(0, 0);
  _lcd->print("Debes $");
  dtostrf(required, 2, 0, buffer);
  _lcd->print(buffer);

  _lcd->setCursor(0, 1);
  _lcd->print("Saldo $");
  dtostrf(current, 2, 0, buffer);
  _lcd->print(buffer);
}

void Screen::showDispensing() {
  _lcd->clear();
  _lcd->setCursor(0, 0);
  _lcd->print("Dispensando...");
}

void Screen::rotateDisplay() {
  // Solo rotar si hay más de 2 productos
  if (Product::getProductCount() > 2) {
    _startIndex = (_startIndex + 2) % Product::getProductCount();
    if (_startIndex + 1 >= Product::getProductCount()) {
      _startIndex = 0;  // Volver al inicio si no quedan suficientes productos
    }
  }
}
void Screen::showError(const String& message) {
  _lcd->clear();
  _lcd->setCursor(0, 0);
  _lcd->print("Error:");
  _lcd->setCursor(0, 1);
  _lcd->print(message);
}