// machine.h
#ifndef MACHINE_H
#define MACHINE_H

#include "screen.h"
#include "keypad.h"
#include "menu.h"
#include "payment.h"
#include "dispenser.h"

// Estados posibles del sistema
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
  // Estructura para manejar estados
  struct StateHandler {
    void (Machine::*onEnter)();
    void (Machine::*onKey)(char);
    void (Machine::*onExit)();
    void (Machine::*onUpdate)();
  };

  // Componentes principales
  SystemState _state;
  Screen* _screen;
  Keypad* _keys;
  Menu* _productMenu;
  Payment* _cashier;
  Dispenser* _vendor;
  String _errorMessage;

  // Control de UI
  bool firstTimeAdd;
  bool firstTimeDel;

  // Mapeo de teclas a productos
  struct KeyProductMapping {
    char key;
    Product::ProductId productId;
  };
  static const uint8_t MAX_PRODUCT_KEYS = 10;  // A-J máximo
  KeyProductMapping _productKeys[MAX_PRODUCT_KEYS];
  uint8_t _numProductKeys;

  // Tabla de handlers de estado
  static const StateHandler STATE_HANDLERS[];

  // --- Handlers de estado ---
  // IDLE
  void onIdleEnter();
  void onIdleKey(char key);
  void onIdleExit();
  void onIdleUpdate();

  // PRODUCT_SELECTION
  void onProductSelectionEnter();
  void onProductSelectionKey(char key);
  void onProductSelectionExit();
  void onProductSelectionUpdate();

  // PRODUCT_DELETION
  void onProductDeletionEnter();
  void onProductDeletionKey(char key);
  void onProductDeletionExit();
  void onProductDeletionUpdate();

  // CONFIRM_ORDER
  void onConfirmOrderEnter();
  void onConfirmOrderKey(char key);
  void onConfirmOrderExit();
  void onConfirmOrderUpdate();

  // PAYMENT
  void onPaymentEnter();
  void onPaymentKey(char key);
  void onPaymentExit();
  void onPaymentUpdate();

  // DISPENSING
  void onDispensingEnter();
  void onDispensingKey(char key);
  void onDispensingExit();
  void onDispensingUpdate();

  // ERROR
  void onErrorEnter();
  void onErrorKey(char key);
  void onErrorExit();
  void onErrorUpdate();

  // --- Métodos auxiliares ---
  void initializeProductKeyMappings();
  bool handleProductKey(char key, bool isAddition);
  Product::ProductId getProductIdForKey(char key) const;
  bool isProductKey(char key) const;
  void changeState(SystemState newState);
  void updateDisplay();
  void setError(const String& message);

public:
  // Constructor y métodos principales
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