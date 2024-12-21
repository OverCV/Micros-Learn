// machine.cpp
#include "machine.h"

// Definición de la tabla de handlers de estado
const Machine::StateHandler Machine::STATE_HANDLERS[] = {
  // IDLE
  { &Machine::onIdleEnter, &Machine::onIdleKey, &Machine::onIdleExit, &Machine::onIdleUpdate },
  // PRODUCT_SELECTION
  { &Machine::onProductSelectionEnter, &Machine::onProductSelectionKey, &Machine::onProductSelectionExit, &Machine::onProductSelectionUpdate },
  // PRODUCT_DELETION
  { &Machine::onProductDeletionEnter, &Machine::onProductDeletionKey, &Machine::onProductDeletionExit, &Machine::onProductDeletionUpdate },
  // CONFIRM_ORDER
  { &Machine::onConfirmOrderEnter, &Machine::onConfirmOrderKey, &Machine::onConfirmOrderExit, &Machine::onConfirmOrderUpdate },
  // PAYMENT
  { &Machine::onPaymentEnter, &Machine::onPaymentKey, &Machine::onPaymentExit, &Machine::onPaymentUpdate },
  // DISPENSING
  { &Machine::onDispensingEnter, &Machine::onDispensingKey, &Machine::onDispensingExit, &Machine::onDispensingUpdate },
  // ERROR
  { &Machine::onErrorEnter, &Machine::onErrorKey, &Machine::onErrorExit, &Machine::onErrorUpdate }
};

// Constructor y métodos principales
Machine::Machine(Screen* screen, Keypad* keys, Menu* productMenu, Payment* cashier, Dispenser* vendor)
  : _state(SystemState::IDLE), _screen(screen), _keys(keys), _productMenu(productMenu), _cashier(cashier), _vendor(vendor), firstTimeAdd(true), firstTimeDel(true), _numProductKeys(0) {
}

void Machine::init() {
  initializeProductKeyMappings();
  _screen->showWelcome();
  _keys->init();
  _vendor->init();
  (this->*STATE_HANDLERS[static_cast<int>(_state)].onEnter)();
}

void Machine::run() {
  (this->*STATE_HANDLERS[static_cast<int>(_state)].onUpdate)();

  // Solo actualizar rotación en estados de selección/borrado de productos
  if (_state == SystemState::PRODUCT_SELECTION || _state == SystemState::PRODUCT_DELETION) {
    _screen->updateRotation();
  }

  char key = _keys->getKey();
  if (key != '\0') {
    Serial.print(F("Tecla presionada: "));
    Serial.println(key);
    (this->*STATE_HANDLERS[static_cast<int>(_state)].onKey)(key);
  }
}

// Handlers de IDLE
void Machine::onIdleEnter() {
  _screen->showWelcome();
}

void Machine::onIdleKey(char key) {
  if (key == '>') {
    changeState(SystemState::PRODUCT_SELECTION);
  }
}

void Machine::onIdleExit() {}
void Machine::onIdleUpdate() {}

// Handlers de PRODUCT_SELECTION
void Machine::onProductSelectionEnter() {
  if (firstTimeAdd) {
    _screen->showAdditionOptions();
    firstTimeAdd = false;
  }
  _screen->showCart(_productMenu);
}

void Machine::onProductSelectionKey(char key) {
  if (isProductKey(key)) {
    if (handleProductKey(key, true)) {
      updateDisplay();
    }
    return;
  }

  switch (key) {
    case '<':
      if (_productMenu->hasProducts()) {
        changeState(SystemState::PRODUCT_DELETION);
      }
      break;
    case '>':
      if (_productMenu->hasProducts()) {
        changeState(SystemState::CONFIRM_ORDER);
      }
      break;
  }
}

void Machine::onProductSelectionExit() {
  firstTimeAdd = true;
}
void Machine::onProductSelectionUpdate() {}

// Handlers de PRODUCT_DELETION
void Machine::onProductDeletionEnter() {
  if (firstTimeDel) {
    _screen->showDeletionOptions();
    firstTimeDel = false;
  }
  _screen->showCart(_productMenu);
}

void Machine::onProductDeletionKey(char key) {
  if (isProductKey(key)) {
    if (handleProductKey(key, false)) {
      updateDisplay();
    }
    return;
  }

  switch (key) {
    case '<':
      changeState(SystemState::PRODUCT_SELECTION);
      break;
    case '>':
      changeState(SystemState::CONFIRM_ORDER);
      break;
  }
}

void Machine::onProductDeletionExit() {
  firstTimeDel = true;
}
void Machine::onProductDeletionUpdate() {}

// Handlers de CONFIRM_ORDER
void Machine::onConfirmOrderEnter() {
  _screen->showConfirmation(_productMenu->getTotal());
}

void Machine::onConfirmOrderKey(char key) {
  switch (key) {
    case '<':
      changeState(SystemState::PRODUCT_SELECTION);
      break;
    case '>':
      _cashier->setRequired(_productMenu->getTotal());
      changeState(SystemState::PAYMENT);
      break;
  }
}

void Machine::onConfirmOrderExit() {}
void Machine::onConfirmOrderUpdate() {}

// Handlers de PAYMENT
void Machine::onPaymentEnter() {
  updateDisplay();
}
void Machine::onPaymentKey(char key) {
  if (key == '>') {
    if (_cashier->isComplete()) {
      changeState(SystemState::DISPENSING);
    }
    return;
  }

  if (_cashier->insertCoinByKey(key)) {
    updateDisplay();
  }
}

void Machine::onPaymentExit() {}

void Machine::onPaymentUpdate() {
  _cashier->checkInterrupts();  // antes era checkCoins()
  updateDisplay();
}
// Handlers de DISPENSING
void Machine::onDispensingEnter() {
  _screen->showDispensing();
}

void Machine::onDispensingKey(char key) {}

void Machine::onDispensingExit() {
  _productMenu->clearCart();
  _cashier->reset();
}

void Machine::onDispensingUpdate() {
  const Menu::CartItem* cart = _productMenu->getCart();
  uint8_t cartSize = _productMenu->getSize();

  for (uint8_t i = 0; i < cartSize; i++) {
    for (uint8_t j = 0; j < cart[i].quantity; j++) {
      _vendor->dispense(cart[i].id);
    }
  }

  changeState(SystemState::IDLE);
}

// Handlers de ERROR
void Machine::onErrorEnter() {
  _screen->showError(_errorMessage);
}

void Machine::onErrorKey(char key) {
  if (key == '0') {
    _productMenu->clearCart();
    _cashier->reset();
    _vendor->reset();
    changeState(SystemState::IDLE);
  }
}

void Machine::onErrorExit() {}
void Machine::onErrorUpdate() {}

// Métodos auxiliares
void Machine::initializeProductKeyMappings() {
  _numProductKeys = 0;
  for (uint8_t i = 0; i < Product::getProductCount() && _numProductKeys < MAX_PRODUCT_KEYS; i++) {
    const Product::ProductInfo* info = Product::getProductInfo(i + 1);
    if (info && info->available) {
      _productKeys[_numProductKeys].key = info->displayKey;
      _productKeys[_numProductKeys].productId = info->id;
      _numProductKeys++;
    }
  }
}

bool Machine::handleProductKey(char key, bool isAddition) {
  Product::ProductId productId = getProductIdForKey(key);
  if (productId == 0) return false;

  if (isAddition) {
    return _productMenu->addToCart(productId);
  } else {
    return _productMenu->removeFromCart(productId);
  }
}

Product::ProductId Machine::getProductIdForKey(char key) const {
  for (uint8_t i = 0; i < _numProductKeys; i++) {
    if (_productKeys[i].key == key) {
      return _productKeys[i].productId;
    }
  }
  return 0;
}

bool Machine::isProductKey(char key) const {
  for (uint8_t i = 0; i < _numProductKeys; i++) {
    if (_productKeys[i].key == key) {
      return true;
    }
  }
  return false;
}

void Machine::changeState(SystemState newState) {
  (this->*STATE_HANDLERS[static_cast<int>(_state)].onExit)();
  _state = newState;
  (this->*STATE_HANDLERS[static_cast<int>(_state)].onEnter)();
}

void Machine::updateDisplay() {
  switch (_state) {
    case SystemState::IDLE:
      _screen->showWelcome();
      break;
    case SystemState::PRODUCT_SELECTION:
      _screen->showCart(_productMenu);
      break;
    case SystemState::PRODUCT_DELETION:
      _screen->showCart(_productMenu);
      break;
    case SystemState::CONFIRM_ORDER:
      _screen->showConfirmation(_productMenu->getTotal());
      break;
    case SystemState::PAYMENT:
      _screen->showPayment(_cashier->getRemaining(),
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

void Machine::setError(const String& message) {
  _errorMessage = message;
  changeState(SystemState::ERROR);
}