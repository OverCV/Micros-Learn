// machine.h
#ifndef MACHINE_H
#define MACHINE_H

#include "screen.h"
#include "keypad.h"
#include "menu.h"
#include "payment.h"
#include "dispenser.h"

enum class SystemState {
  IDLE,
  PRODUCT_SELECTION,
  PRODUCT_DELETION,
  CONFIRM_ORDER,
  PAYMENT,
  DISPENSING,
  ERROR
};

class Machine {
private:
  SystemState _state;

  Screen* _screen;
  Keypad* _keys;
  Menu* _productMenu;
  Payment* _cashier;
  Dispenser* _vendor;

  // Estados anteriores para manejo de errores
  SystemState _previousState;
  String _errorMessage;

  // Para manejo de única visualización
  bool firstTimeAdd = true;
  bool firstTimeDel = true;

  // Métodos de manejo de estados
  void handleIdleState(char key);
  void handleProductSelection(char key);
  void handleProductDeletion(char key);
  void handleConfirmOrder(char key);
  void handlePayment(char key);
  void handleDispensing();
  void handleError(char key);

  // Métodos auxiliares
  void updateDisplay();
  void transitionTo(SystemState newState);
  void setError(const String& message);

public:
  Machine(Screen* screen,
          Keypad* keys,
          Menu* productMenu,
          Payment* cashier,
          Dispenser* vendor);

  void init();
  void run();
  SystemState getState() const {
    return _state;
  }
};

#endif
