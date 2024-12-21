#include "payment.h"

Payment::Payment(volatile bool& coin100, volatile bool& coin200, volatile bool& coin500,
                 uint8_t ledCoin100, uint8_t ledCoin200, uint8_t ledCoin500)
  : _paid(0), _required(0), _lastCoin(0), _needsReturn(false),
    _coin100(coin100), _coin200(coin200), _coin500(coin500),
    _ledCoin100(ledCoin100), _ledCoin200(ledCoin200), _ledCoin500(ledCoin500) {
  // Configurar pines de LEDs
  pinMode(_ledCoin100, OUTPUT);
  pinMode(_ledCoin200, OUTPUT);
  pinMode(_ledCoin500, OUTPUT);
  reset();
}

void Payment::checkCoins() {
  if (_coin100) {
    insertCoin(0);
    _coin100 = false;
  }
  if (_coin200) {
    insertCoin(1);
    _coin200 = false;
  }
  if (_coin500) {
    insertCoin(2);
    _coin500 = false;
  }
}

bool Payment::isValidCoin(uint8_t index) const {
  return index < NUM_COINS;
}

bool Payment::insertCoin(uint8_t coinIndex) {
  if (!isValidCoin(coinIndex)) {
    Serial.println(F("Error: Moneda inválida"));
    return false;
  }

  _lastCoin = coinIndex;
  uint16_t newAmount = _paid + COINS[coinIndex];

  Serial.print(F("Insertando moneda de valor: "));
  Serial.println(COINS[coinIndex]);
  Serial.print(F("Monto total propuesto: "));
  Serial.println(newAmount);

  if (newAmount > _required) {
    Serial.println(F("Error: Monto excede lo requerido"));
    _needsReturn = true;
    blinkCoinLED(coinIndex);
    _needsReturn = false;
    return false;
  }

  _needsReturn = false;
  _paid = newAmount;

  Serial.print(F("Monto actualizado: "));
  Serial.println(_paid);

  return true;
}


void Payment::blinkCoinLED(uint8_t coinIndex) {
  uint8_t ledPin = 0;

  // Asociar índice de moneda con el pin correspondiente
  switch (coinIndex) {
    case 0:
      ledPin = _ledCoin100;
      break;
    case 1:
      ledPin = _ledCoin200;
      break;
    case 2:
      ledPin = _ledCoin500;
      break;
  }

  // Encender y apagar el LED asociado
  digitalWrite(ledPin, HIGH);
  delay(500);  // 500 ms de encendido
  digitalWrite(ledPin, LOW);
}

void Payment::setRequired(uint16_t amount) {
  _required = amount;
  reset();
}

void Payment::previewRequired(uint16_t amount) {
  _required = amount;
  Serial.print(F("Vista previa del monto requerido: "));
  Serial.println(_required);
}

void Payment::reset() {
  _paid = 0;
  _needsReturn = false;
  _lastCoin = 0;

  // Asegurarse de apagar todos los LEDs
  digitalWrite(_ledCoin100, LOW);
  digitalWrite(_ledCoin200, LOW);
  digitalWrite(_ledCoin500, LOW);
}
