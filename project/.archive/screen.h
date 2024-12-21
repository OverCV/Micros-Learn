#include <stdint.h>
// screen.h
#ifndef SCREEN_H
#define SCREEN_H

#include "display.h"
#include "menu.h"
#include "payment.h"

class Screen {
private:
  // Variables para recordar los últimos valores mostrados
  uint16_t lastRequired = 0;
  uint16_t lastCurrent = 0;
  Display* _lcd;

public:
  Screen(Display* lcd);

  void showWelcome();
  void showAdditionOptions();
  void showDeletionOptions();
  void showCart(const Menu* menu);

  void showConfirmation(uint16_t totalAmount);
  void showPayment(uint16_t required, uint16_t current);
  void showAcceptedCoin(uint8_t coinIndex);
  void showDispensing();
  void showError(const String& message);
};

#endif
