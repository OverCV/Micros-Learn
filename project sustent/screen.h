// screen.h
#ifndef SCREEN_H
#define SCREEN_H

#include "display.h"
#include "menu.h"
#include "payment.h"

class Screen {
private:
  Display* _lcd;
  uint16_t lastRequired = 0;
  uint16_t lastCurrent = 0;

  // Variables para la rotación
  uint8_t _startIndex = 0;
  unsigned long _lastRotationTime = 0;
  static const unsigned long ROTATION_INTERVAL = 1000;  // 3 segundos
  const Menu* _currentMenu = nullptr;                   // Para mantener referencia al menú actual

  void printProductLine(const Product::ProductInfo* info, uint8_t quantity, uint8_t line);

public:
  Screen(Display* lcd);

  void showWelcome();
  void showAdditionOptions();
  void showDeletionOptions();
  void showConfirmation(uint16_t totalAmount);
  void showPayment(uint16_t required, uint16_t current);
  void showDispensing();
  void showError(const String& message);
  void showCart(const Menu* menu);

  void rotateDisplay();
  void updateRotation();  // Nuevo método público para actualizar la rotación
};

#endif