// machine.cpp
#include "machine.h"


Machine::Machine(Screen* screen, Keypad* keys, Menu* productMenu,
                 Payment* cashier, Dispenser* vendor)
  : _state(SystemState::IDLE), _screen(screen), _keys(keys),
    _productMenu(productMenu), _cashier(cashier), _vendor(vendor),
    _previousState(SystemState::IDLE) {
}

void Machine::init() {
  _screen->showWelcome();
  _keys->init();
  _vendor->init();
  // updateDisplay();
}

void Machine::run() {
  // _cashier->setRequired(_productMenu->getTotal());
  if (_state == SystemState::PAYMENT) {
    _cashier->checkCoins();
    // updateDisplay();
  }

  char key = _keys->getKey();
  if (key != '\0') {
    switch (_state) {
      case SystemState::IDLE:
        handleIdleState(key);
        break;
      case SystemState::PRODUCT_SELECTION:
        handleProductSelection(key);
        break;
      case SystemState::PRODUCT_DELETION:
        handleProductDeletion(key);
        break;
      case SystemState::CONFIRM_ORDER:
        handleConfirmOrder(key);
        break;
      case SystemState::PAYMENT:
        handlePayment(key);
        break;
      case SystemState::DISPENSING:
        handleDispensing();
        break;
      case SystemState::ERROR:
        handleError(key);
        break;
    }
  }
}

void Machine::updateDisplay() {
  switch (_state) {
    case SystemState::IDLE:
      _screen->showWelcome();
      break;
    case SystemState::PRODUCT_SELECTION:
      if (firstTimeAdd) {
        _screen->showAdditionOptions();
        // Mostrar solo la primera vez
        firstTimeAdd = false;
      }
      _screen->showCart(_productMenu);
      break;
    case SystemState::PRODUCT_DELETION:
      if (firstTimeDel) {
        _screen->showDeletionOptions();
        // Mostrar solo la primera vez
        firstTimeDel = false;
      }
      _screen->showCart(_productMenu);
      break;
    case SystemState::CONFIRM_ORDER:
      _screen->showConfirmation(_productMenu->getTotal());
      break;
    case SystemState::PAYMENT:
      _screen->showPayment(_cashier->getRequiredAmount(),
                           _cashier->getCurrentAmount());
      break;
    case SystemState::DISPENSING:
      _screen->showDispensing();
      break;
    case SystemState::ERROR:
      _screen->showError(_errorMessage);
      break;
  }
}

void Machine::handleIdleState(char key) {
  if (key == '>') {
    transitionTo(SystemState::PRODUCT_SELECTION);
  }
}

void Machine::handleProductSelection(char key) {
  switch (key) {
    case 'A':
      _productMenu->addToCart(Product::WATER);
      break;
    case 'B':
      _productMenu->addToCart(Product::SODA);
      break;
    case '<':
      if (_productMenu->hasProducts()) {
        transitionTo(SystemState::PRODUCT_DELETION);
      }
      break;
    case '>':
      if (_productMenu->hasProducts()) {
        transitionTo(SystemState::CONFIRM_ORDER);
      }
      break;
  }
  updateDisplay();
}

void Machine::handleProductDeletion(char key) {
  switch (key) {
    case 'A':
      _productMenu->removeFromCart(Product::WATER);
      break;
    case 'B':
      _productMenu->removeFromCart(Product::SODA);
      break;
    case '<':
      transitionTo(SystemState::PRODUCT_SELECTION);
      break;
    case '>':
      transitionTo(SystemState::CONFIRM_ORDER);
      break;
  }
  updateDisplay();
}

void Machine::handleConfirmOrder(char key) {
  switch (key) {
    case '<':
      transitionTo(SystemState::PRODUCT_SELECTION);
      break;
    case '>':
      _cashier->setRequired(_productMenu->getTotal());
      transitionTo(SystemState::PAYMENT);
      break;
  }
  updateDisplay();
}

void Machine::handlePayment(char key) {
  Serial.println(F("Manejando estado de pago"));

  // Verificar monedas ingresadas por interrupciones
  _cashier->checkCoins();

  // Verificar monedas ingresadas por teclado
  uint8_t coinIndex = 255;
  switch (key) {
    case '1':
      coinIndex = 0;
      break;
    case '2':
      coinIndex = 1;
      break;
    case '5':
      coinIndex = 2;
      break;
  }

  if (coinIndex != 255) {
    Serial.print(F("Tecla detectada para moneda: "));
    Serial.println(coinIndex);
    if (_cashier->insertCoin(coinIndex)) {
      Serial.println(F("Moneda aceptada"));
    } else {
      Serial.println(F("Moneda no aceptada"));
    }
  }

  if (_cashier->needsReturn()) {
    Serial.println(F("Monto excedente, devolviendo moneda"));
  }

  // if (_cashier->isComplete()) {
  //   Serial.println(F("Pago completo, cambiando a estado DISPENSING"));
  //   transitionTo(SystemState::DISPENSING);
  // }
  switch (key) {
    case '>':
      transitionTo(SystemState::DISPENSING);
      break;
  }
  updateDisplay();
}


void Machine::handleDispensing() {
  // Obtener los productos en el carrito
  const Menu::CartItem* cart = _productMenu->getCart();
  uint8_t cartSize = _productMenu->getSize();

  // Dispensar cada producto en el carrito
  for (uint8_t i = 0; i < cartSize; i++) {
    for (uint8_t j = 0; j < cart[i].quantity; j++) {
      _vendor->dispense(cart[i].type);
    }
  }

  // Limpiar y volver a IDLE
  _productMenu->clearCart();
  _cashier->reset();
  transitionTo(SystemState::IDLE);
}


void Machine::handleError(char key) {
  if (key == '0') {  // Tecla de reset
    _productMenu->clearCart();
    _cashier->reset();
    _vendor->reset();
    transitionTo(SystemState::IDLE);
  }
}


void Machine::transitionTo(SystemState newState) {
  _previousState = _state;
  _state = newState;

  // Manejo de primera vista en productos
  firstTimeAdd = (newState == SystemState::PRODUCT_SELECTION);
  firstTimeDel = (newState == SystemState::PRODUCT_DELETION);

  updateDisplay();
}

void Machine::setError(const String& message) {
  _errorMessage = message;
  transitionTo(SystemState::ERROR);
}
