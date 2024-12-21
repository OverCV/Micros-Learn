#include <stdint.h>
// screen.cpp
#include "screen.h"

Screen::Screen(Display* lcd)
  : _lcd(lcd) {
}

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
  _lcd->clear();

  char buffer[6];  // Para convertir los números a texto

  uint8_t aguaQty = menu->getProductCount(Product::WATER);
  uint8_t sodaQty = menu->getProductCount(Product::SODA);

  _lcd->setCursor(0, 0);
  _lcd->print("[A] Agua: ");
  dtostrf(aguaQty, 2, 0, buffer);
  _lcd->print(buffer);

  _lcd->setCursor(0, 1);
  _lcd->print("[B] Soda: ");
  dtostrf(sodaQty, 2, 0, buffer);
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
    Serial.println(F("Sin cambios en la pantalla de pago"));
    return;
  }

  lastRequired = required;
  lastCurrent = current;

  char buffer[9];
  _lcd->clear();
  _lcd->setCursor(0, 0);
  _lcd->print("Total $");
  dtostrf(required, 2, 0, buffer);
  _lcd->print(buffer);

  _lcd->setCursor(0, 1);
  _lcd->print("Saldo $");
  dtostrf(current, 2, 0, buffer);
  _lcd->print(buffer);

  Serial.print(F("Mostrando Total: "));
  Serial.println(required);
  Serial.print(F("Mostrando Saldo: "));
  Serial.println(current);
}


void Screen::showAcceptedCoin(uint8_t coinIndex) {
  uint8_t valor = 0;

  // Asociar índice de moneda con el pin correspondiente
  switch (coinIndex) {
    case 2:
      valor = 500;
      break;
    case 1:
      valor = 200;
      break;
    default:
      valor = 100;
  }

  char buffer[9];
  dtostrf(valor, 2, 0, buffer);

  _lcd->clear();
  _lcd->setCursor(0, 0);
  _lcd->print("Ingresaste:");
  _lcd->setCursor(0, 1);
  _lcd->print("$");
  _lcd->print(valor);
}

void Screen::showDispensing() {
  _lcd->clear();
  _lcd->setCursor(0, 0);
  _lcd->print("Dispensando...");
}

void Screen::showError(const String& message) {
  _lcd->clear();
  _lcd->setCursor(0, 0);
  _lcd->print("Error:");
  _lcd->setCursor(0, 1);
  _lcd->print(message);
}
